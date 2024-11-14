#pragma once
#include <coroutine>

class async_manual_reset_event
{
public:
  async_manual_reset_event(bool initially_set=false) noexcept;

  //No copying/moving
  async_manual_reset_event(const async_manual_reset_event&)=delete;
  async_manual_reset_event(async_manual_reset_event&&)=delete;
  async_manual_reset_event& operator=(const async_manual_reset_event&)=delete;
  async_manual_reset_event& operator=(async_manual_reset_event&&)=delete;

  bool is_set() const noexcept;
  struct awaiter;
  awaiter operator co_await() const noexcept;

  void set() noexcept;
  void reset() noexcept;
private:

  friend struct awaiter;

  // - 'this' => set state
  // - otherwise => not set, head of linked list of awaiter*
  mutable std::atomic<void*> m_state;
};

struct async_manual_reset_event::awaiter
{
  awaiter(const async_manual_reset_event& event) noexcept
  : m_event(event)
  {}

  bool await_ready() const noexcept;
  bool await_suspend(std::coroutine_handle<> awaiting_coroutine) noexcept;
  void await_resume() noexcept {}
private:
  const async_manual_reset_event& m_event;
  std::coroutine_handle<> m_awaiting_coroutine;
  awaiter* m_next;
};
