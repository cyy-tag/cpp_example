#include "async_manual_reset_event.h"

bool async_manual_reset_event::awaiter::await_ready() const noexcept
{
  return m_event.is_set();
}

bool async_manual_reset_event::awaiter::await_suspend(
  std::coroutine_handle<> awaiting_coroutine) noexcept
{
  // Special m_state value that indicates the event is in the 'set' state.
  const void* const set_state = &m_state;
  m_awaiting_coroutine = awaiting_coroutine;

  void* old_value = m_event.m_state.load(std::memory_order_acquire);
  do
  {
    // Resume immediately if already in 'set' state.
    if(old_value == set_state) return false;

    m_next = static_cast<awaiter*>(old_value);
  } while (!m_event.m_state.compare_exchange_weak(
          old_value,
          this,
          std::memory_order_release,
          std::memory_order_acquire));

  // Successfully enqueued. Remain suspend.
  return true;
}

async_manual_reset_event::async_manual_reset_event(
  bool initially_set) noexcept
  : m_state(initially_set ? this : nullptr)
{}

bool async_manual_reset_event::is_set() const noexcept
{
  return m_state.load(std::memory_order_acquire) == this;
}

void async_manual_reset_event::reset() noexcept
{
  void* old_value = this;
  m_state.compare_exchange_strong(old_value, nullptr, std::memory_order_acquire);
}

void async_manual_reset_event::set() noexcept
{
  //Needs to be 'release' so that subsequent 'co_await' has
  //visibilty of out prior writes.
  void* old_value = m_state.exchange(this, std::memory_order_acq_rel);
  if(old_value != this) {
    auto* waiters = static_cast<awaiter*>(old_value);
    while(waiters != nullptr)
    {
      auto* next = waiters->m_next;
      waiters->m_awaiting_coroutine.resume();
      waiters = next;
    }
  }
}

async_manual_reset_event::awaiter
async_manual_reset_event::operator co_await() const noexcept
{
  return awaiter{*this};
}
