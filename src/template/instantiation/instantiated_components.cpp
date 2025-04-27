template<typename T>
class Safe {

};

template<int N>
class Danger {
  int arr[N];
};

template<typename T, int N>
class Tricky {
public:
  void noBodyHere(Safe<T> = 3); //普通函数只有声明，没有使用不会进行实列化
  void inClass() {
    Danger<N> noBoomYet;
  }
  //nest struct
  struct Nested {
    Danger<N> pfew;
  };

  //union
  union {
    Danger<N> anonymous;
    int align;
  };

  void unsafe(T (*p)[N]);
  void error() {
    Danger<-1> boom;
  }
};

//虚函数
template<typename T>
class VirtualClass {
public:
  virtual ~VirtualClass() {}
  virtual T vmem(); //虚函数无定义会在链接时报错, 这是存在虚函数表，需要函数地址
};

//operator -> 操作, 一般返回类型对象，在这里模板即使返回int类型也不会报错
template<typename T>
class C {
public:
  T operator-> ();
};

int main() {
  Tricky<int,10> inst; //OK, 没有使用到error函数，不会实列化,因此不会报错
  // inst.error(); /进行实例化，-1初始化数组长度编译错误

  // VirtualClass<int> inst2; //存在虚函数类模板实列化，链接报错

  C<int> in;
  return -1;
}
