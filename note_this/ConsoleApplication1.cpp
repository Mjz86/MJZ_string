/*



 CAST from DRIVED to   BASE
 and reversed




*/

#include <iostream>
class P {
 public:
  virtual void ptr() { std::cout << "P:" << this << std::endl; }
};
class A : virtual public P {
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

class C {
 public:
  char m_c{'c'};
  void c() { std::cout << "C::c()" << std::endl; }
  void ptr() { std::cout << "C:" << this << std::endl; }
};
class D {
 public:
  char m_d{'d'};
  void d() { std::cout << "D::d()" << std::endl; }
  void ptr() { std::cout << "D:" << this << std::endl; }
};
class CD : public C, public D {
 public:
  char m_cd{'E'};
  void c() { std::cout << "CD::c()" << std::endl; }
  void d() { std::cout << "CD::d()" << std::endl; }

  void ptr() { std::cout << "CD:" << this << std::endl; }
};
class DC : public D, public C {
 public:
  char m_dc{'F'};
  void c() { std::cout << "DC::c()" << std::endl; }
  void d() { std::cout << "DC::d()" << std::endl; }

  void ptr() { std::cout << "DC:" << this << std::endl; }
};
template <typename T>
void println(const T& obj) {
  std::cout << obj << '\n';
}
int main(int argc, const char* const* const argv) {
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
  std::cout << "number of v tables in AB :" << sizeof(AB) / sizeof(void*)
            << std::endl;
  std::cout << "ptr:" << b << std::endl;
  ((P*)b)->P::ptr();  // p
  // ((A*)b)->A::ptr();//a wrong becuse of compiler magic could cause crash
  ((A*)((AB*)b))->A::ptr();  // a right
  //  ((B*)b)->B::ptr();    // b wrong if this wasent a b* but now its just not
  //  ding any thing
  ((B*)((AB*)b))->B::ptr();  // b right
  ((AB*)b)->AB::ptr();       // ab
  b->b();                    // ab::b

  println((P*)b);
  println((A*)b);         // a wrong becuse of compiler magic could cause crash
  println((A*)((AB*)b));  // a right
  println((B*)b);  // b wrong if this wasent a b* but now its just not  doing
                   // any thing
  println((B*)((AB*)b));  // b right
  println((AB*)b);        // ab
  println(b);             // ab::b

  D* d = new CD();
  std::cout << "sizeof CD :" << sizeof(CD) << std::endl;
  std::cout << "ptr:" << d << std::endl; 
  // ((C*)d)->C::ptr();//c wrong because of compiler magic could cause crash
  ((C*)((CD*)d))->C::ptr();  // c right
  //  ((D*)d)->D::ptr();    // d wrong if this  was not  c d* dut now its just
  //  not ding cny thing
  ((D*)((CD*)d))->D::ptr();  // d right
  ((CD*)d)->CD::ptr();       // cd
  d->d();                    // cd::d

  println((P*)d);
  println((C*)d);         // wrong
  println((C*)((CD*)d));  // right
  println((D*)d);         // d wrong

  println((D*)((CD*)d));  // d right
  println((CD*)d);        // cd
  println(d);             // cd::d
  println("cast from D* to CD* in compiler ");//probobly
  constexpr size_t the_D_offset_from_begining_of_CD = sizeof(C);// the sum of  sizeof classes inherited befor D 
  CD* cd = (CD*)((char*)d - the_D_offset_from_begining_of_CD);
  println(cd);
  println("cast from CD* to D* in compiler ");  // probobly 
  println((D*)((char*)cd + the_D_offset_from_begining_of_CD));



  /////////////////////////////////////////////////////////////////////
  //     
  //
  //                CD:
  //           // C / D / CD//
  //           //'c'/'d'/'E'//
  //           ///////////////
  //            ^    ^
  //            |    |
  //         C_ptr  D_ptr
  //         CD_ptr
  // 
  //          cast CD to C = CDptr
  //          cast CD to D = (D*)((char*)CDptr + sizeof(C))
  // 
  // 
  //          cast C to CD = Cptr
  //          cast D to CD = (CD*)((char*)Dptr - sizeof(C))   ====  (CD*)( (char*)D_ptr+(0 -(D*)((char*)((CD*)0) + sizeof(D))))
  //
  // 
  // 
  // soo  the correct cast from  d=(D*)(&cd) to C* is NOT !!!!  c=(C*)d   !!!! undefined behavior
  //   the correct cast from  d  to C* is   c=(C*)((CD*)d)   
  //
  //
  //
  //
  delete b;
  return 0;
}