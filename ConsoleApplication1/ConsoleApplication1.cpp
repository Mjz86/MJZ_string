// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream> 
class P {
 public:
  virtual void ptr() { std::cout << "P:" << this << std::endl; }
};
class A:virtual public P {
 public:
  virtual void a() { std::cout << "A::a()" << std::endl; }
  virtual ~A() { std::cout << "A::~A()" << std::endl; }

  virtual void ptr() override { std::cout << "A:" << this << std::endl; }

};

class B : virtual public P {
 public:
  virtual void b() { std::cout << "B::b()" << std::endl; }
  virtual ~B() { std::cout << "B::~B()" << std::endl; }
  virtual void ptr() override { std::cout << "B:" << this << std::endl; }

};

class AB : public A, public B {
 public:
  void a() override { std::cout << "AB::a()" << std::endl; }
  void b() override { std::cout << "AB::b()" << std::endl; }
  virtual void c() { std::cout << "AB::c()" << std::endl; }
  virtual ~AB() { std::cout << "AB::~AB()" << std::endl; }
  virtual void ptr() override { std::cout << "AB:" << this << std::endl; }
};

class BA : public B, public A {
 public:
  void a() override { std::cout << "BA::a()" << std::endl; }
  void b() override { std::cout << "BA::b()" << std::endl; }
  virtual void d() { std::cout << "BA::d()" << std::endl; }
  virtual ~BA() { std::cout << "BA::~BA()" << std::endl; }
  virtual void ptr() override { std::cout << "BA:" << this << std::endl; }
};

int  main(int argc, const char* const* const argv) {
  /*
    AB ab;
  ab.a();  // Calls AB::a()
  ab.b();  // Calls AB::b()
  ab.c();  // Calls AB::c()

  BA ba;
  ba.a();  // Calls BA::a()
  ba.b();  // Calls BA::b()
  ba.d();  // Calls BA::d()
  B* b{};
  A* a{};
  b = &ba;
  a = &ba;
  b->b();
  a->a();
  b = &ab;
  a = &ab;
  b->b();
  a->a();
  */
  B* b = new AB();
  std::cout << "sizeof AB :" << sizeof(AB) << std::endl;
  std::cout << "number of v tables in AB :" << sizeof(AB)/sizeof(void*) << std::endl;
  std::cout << "ptr:" << b << std::endl;
  ((P*)b)->P::ptr();    // p
 // ((A*)b)->A::ptr();//a wrong becuse of compiler magic could cause crash 
  ((A*)((AB*)b))->A::ptr();  // a right
//  ((B*)b)->B::ptr();    // b wrong if this wasent a b* but now its just not ding any thing
  ((B*)((AB*)b))->B::ptr();  // b right
  ((AB*)b)->AB::ptr();  // ab
  b->b();//ab::b
  delete b;
  return 0;
}