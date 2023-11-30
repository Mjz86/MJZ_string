#include <iostream>

#include "my_main.h"

class A {
 public:
  virtual void a() { std::cout << "A::a()" << std::endl; }
  virtual ~A() { std::cout << "A::~A()" << std::endl; }
};

class B {
 public:
  virtual void b() { std::cout << "B::b()" << std::endl; }
  virtual ~B() { std::cout << "B::~B()" << std::endl; }
};

class AB : public A, public B {
 public:
  void a() override { std::cout << "AB::a()" << std::endl; }
  void b() override { std::cout << "AB::b()" << std::endl; }
  virtual void c() { std::cout << "AB::c()" << std::endl; }
  virtual ~AB() { std::cout << "AB::~AB()" << std::endl; }
};

class BA : public B, public A {
 public:
  void a() override { std::cout << "BA::a()" << std::endl; }
  void b() override { std::cout << "BA::b()" << std::endl; }
  virtual void d() { std::cout << "BA::d()" << std::endl; }
  virtual ~BA() { std::cout << "BA::~BA()" << std::endl; }
};

int my_main::main(int argc, const char* const* const argv) {
  using namespace mjz_ard::have_mjz_ard_removed;
  using namespace mjz_ard;

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

std::cout << "i am main \n";
  return 0;
}
