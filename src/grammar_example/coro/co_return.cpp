#include <concepts>
#include <coroutine>
#include <exception>
#include <iostream>
/*
 * co_return;
 * -> call promise.return_void();
 * co_return <expr>
 * -> call promise.return_void(<expr>) if <expr> does not have type void
 * -> <expr>; promise.return_void(); if <expr> has type void
 * 
 * 如果 promise.return_void抛出异常 会传递调用
 * promise.unhandle_exception()
 * 
 */

