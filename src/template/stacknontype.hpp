#pragma once

#include <array>
#include <cassert>

template<typename T, std::size_t Maxsize>
class Stack {
  private:
    std::array<T, Maxsize> elems_;
    std::size_t num_elems_;
  public:
    Stack();
    void Push(T const& elem);
    void Pop();
    T const& Top() const;
    bool Empty() const {
      return num_elems_ == 0;
    }
    std::size_t Size() const {
      return num_elems_;
    }
};

template<typename T, std::size_t Maxsize>
Stack<T,Maxsize>::Stack()
 : num_elems_(0)
{

}

template<typename T, std::size_t Maxsize>
void Stack<T,Maxsize>::Push(T const& elem)
{
  assert(num_elems_ < Maxsize);
  elems_[num_elems_] = elem;
  ++num_elems_;
}

template<typename T, std::size_t Maxsize>
void Stack<T, Maxsize>::Pop()
{
  assert(!elems_.empty());
  --num_elems_;
}

template<typename T, std::size_t Maxsize>
T const& Stack<T,Maxsize>::Top() const
{
  assert(!elems_.empty());
  return elems_[num_elems_-1];
}
