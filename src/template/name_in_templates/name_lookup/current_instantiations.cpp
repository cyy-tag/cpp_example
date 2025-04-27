/*Current Instantiations (当前实列化名)*/
//除了在普通类中注入类名，也可以在类模板中注入实例化名
//在类模板（成员函数，内嵌类）中，有些名称可以推导为当前实列化名
//这样做的好处：可以在定义的时候可以检测和探测错误，而不是在实列化时候。

template<class T>
class A
{
    A* p1;      // A is the current instantiation
    A<T>* p2;   // A<T> is the current instantiation
    ::A<T>* p4; // ::A<T> is the current instantiation
    A<T*> p3;   // A<T*> is not the current instantiation
 
    class B
    {
        B* p1;                 // B is the current instantiation
        A<T>::B* p2;           // A<T>::B is the current instantiation
        typename A<T*>::B* p3; // A<T*>::B is not the current instantiation
    };
};
