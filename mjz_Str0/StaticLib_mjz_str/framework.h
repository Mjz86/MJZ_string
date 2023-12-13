#pragma once

#include <chrono>
#include <cstdint>
#include <iostream>

#include "mjzString_helper.hpp"

namespace mjz_ard {

class speed_Timer {
 public:
  speed_Timer() { Reset(); }

  void Reset() { m_Start = std::chrono::high_resolution_clock::now(); }

  long double Elapsednano() {
    return (long double)std::chrono::duration_cast<std::chrono::nanoseconds>(
               std::chrono::high_resolution_clock::now() - m_Start)
        .count();
  }
  long double Elapsed() { return Elapsednano() * 0.001f * 0.001f * 0.001f; }

  long double ElapsedMillis() { return Elapsednano() / 1000.0f / 1000.0f; }

 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
};
class tiny_scoped_timer {
 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
  const char*  str = "";

 public:
  tiny_scoped_timer(const char* s)
      : m_Start(std::chrono::high_resolution_clock::now()), str(s) {

  }
  ~tiny_scoped_timer() { 
      size_t time = (std::chrono::duration_cast<std::chrono::nanoseconds>(
                       std::chrono::high_resolution_clock::now() - m_Start)
                       .count());
 std::cout << str << " : " << time<< " ns\n";
  }

};
struct timer_info {
  long double Time_ns;
  long double atempt_num;
};
inline bool operator<(timer_info rhs, timer_info lhs) {
  return (rhs.Time_ns / rhs.atempt_num) < (lhs.Time_ns / lhs.atempt_num);
}
enum timer_cmd : uint8_t {
  NONE = 0,
  Force_log = MJZ_logic_BIT(0),
  NO_Stop = MJZ_logic_BIT(1),
  just_Stop = MJZ_logic_BIT(2),
};
class Scoped_speed_Timer {
 public:
  void set_list(std::shared_ptr<std::map<std::string, timer_info>> vect_) {
    m_uomp = std::move(vect_);
  }
  Scoped_speed_Timer(const std::string& name, bool glob = 1) : m_Name(name) {
    if (glob) {
      m_uomp = g_map;
    }
  }
  Scoped_speed_Timer(const char* name, size_t size_, bool glob = 1)
      : m_Name(name, size_) {
    if (glob) {
      m_uomp = g_map;
    }
  }
  Scoped_speed_Timer(const char* name, bool glob = 1)
      : m_Name(name, strlen(name)) {
    if (glob) {
      m_uomp = g_map;
    }
  }
  Scoped_speed_Timer& operator()(const std::string& name, bool nomatter = 1);
  void Stop(timer_cmd cmd_ = timer_cmd::NONE) {
    long double time = m_Timer.Elapsednano();

    if ((stoped && !(cmd_ & timer_cmd::Force_log)) ||
        !!(cmd_ & timer_cmd::just_Stop)) {
    } else {
      if (m_uomp) {
        timer_info& ti_ = (*m_uomp)[m_Name];
        ti_.atempt_num++;
        ti_.Time_ns += time;
      } else {
        std::cout << "[TIMER] " << m_Name << " - " << time << "ns\n";
      }
    }

    if (!(cmd_ & timer_cmd::NO_Stop)) {
      stoped = 1;
    }

    return;
  }
  speed_Timer& get_timer() { return m_Timer; }
  ~Scoped_speed_Timer() { Stop(); }
  static inline bool operator_less_than(
      const std::pair<std::string, timer_info>& rhs,
      const std::pair<std::string, timer_info>& lhs) {
    return !operator<(rhs.second, lhs.second);
  }
  static std::string show_analysis(
      std::shared_ptr<std::map<std::string, timer_info>> map_ptr,
      const std::string& timer_signuchure_name = "_timer_",
      std::function<bool(const std::pair<std::string, timer_info>&,
                         const std::pair<std::string, timer_info>&)>
          cmpr_fnction = operator_less_than) {
    std::string ret_str;
    auto& _timer_info = (*map_ptr)[timer_signuchure_name];
    long double time_defult = _timer_info.Time_ns / _timer_info.atempt_num;
    std::vector<std::pair<std::string, timer_info>> vect_analis(
        map_ptr->size());
    {
      size_t i{};

      for (auto& obj : *map_ptr) {
        if (obj.first != timer_signuchure_name)
          obj.second.Time_ns =
              (obj.second.Time_ns) - (obj.second.atempt_num * time_defult);

        obj.second.Time_ns = floor(obj.second.Time_ns);
        vect_analis[i++] = obj;
      }

      std::sort(vect_analis.begin(), vect_analis.end(), cmpr_fnction);
      i = 0;
      long double time_token_total{};

      for (auto& obj : vect_analis) {
        long double time_token = (obj.second.Time_ns) / obj.second.atempt_num;
        ret_str += std::to_string(++i);
        ret_str += ". ";
        ret_str += obj.first;
        ret_str += ":";
        ret_str += std::to_string(time_token);
        ret_str += " ns (";
        ret_str += std::to_string(obj.second.Time_ns);
        ret_str += " / ";
        ret_str += std::to_string(obj.second.atempt_num);
        ret_str += ")\n";
        obj.second.Time_ns = time_token;
        time_token_total += time_token;
        obj.second.atempt_num = 1;
      }

      i = 0;
      time_token_total /= 100;

      for (auto& obj : vect_analis) {
        ret_str += std::to_string(++i);
        ret_str += ". ";
        ret_str += obj.first;
        ret_str += ":";
        ret_str += std::to_string((obj.second.Time_ns) / time_token_total);
        ret_str += " %\n";
      }
    }
    return ret_str;
  }

  Scoped_speed_Timer& operator=(const Scoped_speed_Timer& name) {
    (*this)(name.m_Name);

    if (name.m_uomp) {
      m_uomp = name.m_uomp;
    }

    return *this;
  }
  Scoped_speed_Timer(const Scoped_speed_Timer& name) {
    (*this)(name.m_Name);

    if (name.m_uomp) {
      m_uomp = name.m_uomp;
    }
  }
  Scoped_speed_Timer& operator=(Scoped_speed_Timer&& name) noexcept {
    (*this)(name.m_Name);
    m_Timer = name.m_Timer;

    if (name.m_uomp) {
      m_uomp = name.m_uomp;
    }

    return *this;
  }
  Scoped_speed_Timer(Scoped_speed_Timer&& name) noexcept {
    (*this)(name.m_Name);
    m_Timer = name.m_Timer;

    if (name.m_uomp) {
      m_uomp = name.m_uomp;
    }
  }
  static inline void set_global_map(
      std::shared_ptr<std::map<std::string, timer_info>> g_map_) {
    g_map = g_map_;
  }
  static inline std::shared_ptr<std::map<std::string, timer_info>>
  get_global_map() {
    return g_map;
  }

 protected:
  bool stoped{};
  std::string m_Name;
  static std::shared_ptr<std::map<std::string, timer_info>> g_map;
  std::shared_ptr<std::map<std::string, timer_info>> m_uomp;
  speed_Timer m_Timer;
};
class vr_Scoped_speed_Timer : public Scoped_speed_Timer {
  vr_Scoped_speed_Timer(const std::string& name) : Scoped_speed_Timer(name) {}
  vr_Scoped_speed_Timer(const Scoped_speed_Timer& name)
      : Scoped_speed_Timer(name) {}
  vr_Scoped_speed_Timer(Scoped_speed_Timer&& name)
      : Scoped_speed_Timer(std::move(name)) {}
  vr_Scoped_speed_Timer(const vr_Scoped_speed_Timer& name)
      : Scoped_speed_Timer(name) {}
  vr_Scoped_speed_Timer(vr_Scoped_speed_Timer&& name) noexcept
      : Scoped_speed_Timer(std::move(name)) {}
  vr_Scoped_speed_Timer& operator=(const vr_Scoped_speed_Timer& s_tmr) {
    *((Scoped_speed_Timer*)this) = s_tmr;
    return *this;
  }
  vr_Scoped_speed_Timer& operator=(vr_Scoped_speed_Timer&& s_tmr) noexcept {
    *((Scoped_speed_Timer*)this) = std::move(s_tmr);
    return *this;
  }
  vr_Scoped_speed_Timer& operator=(const Scoped_speed_Timer& s_tmr) {
    *((Scoped_speed_Timer*)this) = s_tmr;
    return *this;
  }
  vr_Scoped_speed_Timer& operator=(Scoped_speed_Timer&& s_tmr) {
    *((Scoped_speed_Timer*)this) = std::move(s_tmr);
    return *this;
  }

  virtual ~vr_Scoped_speed_Timer() = default;
};
class std_stream_class_warper {
  std_stream_class_warper() {}

 public:
  template <typename T>
  inline friend std_stream_class_warper& operator<<(
      std_stream_class_warper& COUT, T&& obj) {
    std::cout << std::forward<T>(obj);
    return COUT;
  }

  inline std_stream_class_warper& write(const char* s, size_t l) {
    std::cout.write(s, l);
    return *this;
  }
  static inline std_stream_class_warper& get() {
    static std_stream_class_warper singleton;
    return singleton;
  }
};

template <class counter_class, class my_stream = std_stream_class_warper>
class mjz_class_operation_reporter_t {
  static counter_class index;
  template <typename... argT>
  mjz_class_operation_reporter_t& println(argT&&... args)const {
    print_c_str(std::forward<argT>(args)...);
    print('\n');
    return none();
  }
  template <typename... argT>
  mjz_class_operation_reporter_t& println_wf(const 
      mjz_class_operation_reporter_t& obj, argT&&... args) const {
    print(std::forward<argT>(args)...);
    print_c_str(" with ID: ");
    print(UUID());
    print_c_str(" from ID:");
    print(obj.UUID());
    print('\n');

    return none();
  }
  template <typename... argT>
  mjz_class_operation_reporter_t& println_w(argT&&... args) const {
    print_c_str(std::forward<argT>(args)...);
    print_c_str(" with ID: ");
    print(UUID());
    print('\n');
    return none();
  }
  template <typename... argT>
  mjz_class_operation_reporter_t& println_wi(const counter_class& index,
                                             argT&&... args) const {
    print_c_str(std::forward<argT>(args)...);
    print(index);
    print_c_str(" with ID: ");
    print(UUID());
    print('\n');
    return none();
  }
  template <typename... argT>
  mjz_class_operation_reporter_t& printsv(argT&&... args) const {
    std::initializer_list<std::string_view> list = {
        std::forward<argT>(args)...};
    for (auto& s : list) print(s);
    return none();
  }
  template <typename... argT>
  mjz_class_operation_reporter_t& print_c_str(argT&&... args)const {
    std::initializer_list<const char*> list = {std::forward<argT>(args)...};
    for (auto& s : list) print(s);
    return none();
  }
  mjz_class_operation_reporter_t& print_c_str(const char* s) const {
    print_c_str_len_1(s, strlen(s));
    return none();
  }
  template <typename... argT>
  mjz_class_operation_reporter_t& print(argT&&... args) const {
    auto list =
        std::initializer_list<std::function<void(void)>>{[&](void) -> void {
          print(std::forward<argT>(args));
          return;
        }...};  // do all tasks in thr rigth order
    for (auto& f : list) f();
    return none();
  }
  template <typename T>
  mjz_class_operation_reporter_t& print(T&& arg) const {
    my_stream::get() << std::forward<T>(arg);
    return none();
  }
  mjz_class_operation_reporter_t& print_c_str_len_1(const char* s,
                                                    size_t l) const {
    my_stream::get().write(s, l);
    return none();
  }

  template <typename T>
  mjz_class_operation_reporter_t& println_o(T&& arg) const {
    println_w("operator", std::forward<T>(arg), "()");
    return none();
  }
  template <typename T>
  mjz_class_operation_reporter_t& println_oi(T&& arg) const {
    println_w("operator", std::forward<T>(arg), "(int)");
    return none();
  }
  template <typename T>
  mjz_class_operation_reporter_t& println_obj(
      const mjz_class_operation_reporter_t& obj, T&& arg) const {  // oobj
    print_c_str("  ID: (");
    print(UUID());
    print(" ) operator", std::forward<T>(arg), "(const obj&)", "  ID: ( ");
    print(obj.UUID());
    print(" )\n");
    return none();
  }

 public:
mutable  char filler = '|';
  inline const void* UUID() const { return this; };
  mjz_class_operation_reporter_t() { println_wi(index++, " created : "); }
  mjz_class_operation_reporter_t(int i) : filler(i) {
    print_c_str(" created with int :(");
    print(i);
    println_wi(index++, ")  ");
  }
  mjz_class_operation_reporter_t(char c) : filler(c) {
    print_c_str(" created with char :(");
    print(c);
    println_wi(index++, ")  ");
  }
  mjz_class_operation_reporter_t(const char* s) {
    print_c_str(" created with str : \"");
    println_wi(index++, s, "\"   ");
  }

  mjz_class_operation_reporter_t(std::initializer_list<const char*> args) {
    print_c_str(" created with c_str list : { ");
    auto it = args.begin(), end = args.end();
    if (it == end) {
      println_wi(index++, " }   ");
      return;
    }
    for (;;) {
      print("\"", *it, "\"");
      it++;
      if (it < end) {
        print_c_str_len_1(" , ", 3);
        continue;
      }
      break;
    }
    println_wi(index++, " }   ");
  }
  mjz_class_operation_reporter_t(std::initializer_list<std::string_view> args) {
    print_c_str(" created with string_view list : { ");
    auto it = args.begin(), end = args.end();
    if (it == end) {
      println_wi(index++, " }   ");
      return;
    }
    for (;;) {
      print("\"", *it, "\"");
      it++;
      if (it < end) {
        print_c_str_len_1(" , ", 3);
        continue;
      }
      break;
    }
    println_wi(index++, " }   ");
  }

  mjz_class_operation_reporter_t(const char* s, size_t len) {
    print_c_str(" created with str , len  : \"");
    print_c_str_len_1(s, len);
    println_wi(index++, "\"   ");
  }
  ~mjz_class_operation_reporter_t() { println_wi(--index, " destroyed : "); }
  mjz_class_operation_reporter_t(mjz_class_operation_reporter_t&& obj) {
    index++;
    println_wf(obj, " move constructed ");
  }
  mjz_class_operation_reporter_t& operator=(
      mjz_class_operation_reporter_t&& obj) {
    println_wf(obj, " moved to me  ");
    return none();
  }
  mjz_class_operation_reporter_t(const mjz_class_operation_reporter_t& obj) {
    index++;
    println_wf(obj, " copy constructed ");
  }
  mjz_class_operation_reporter_t& operator=(
      const mjz_class_operation_reporter_t& obj) const {
    println_wf(obj, " copied to me   ");
    return none();
  }
  mjz_class_operation_reporter_t& operator()() const { return println_o("()"); }
  mjz_class_operation_reporter_t& operator()(std::string_view v) const {
    print("operator()(string_view) : (");
    print(v);
    return print_c_str_len_1(") ", 2);
  }
  mjz_class_operation_reporter_t& operator()(const char* s) const {
    print("operator()(c_str) : (");
    print(s);
    return print_c_str_len_1(") ", 2);
  }
  mjz_class_operation_reporter_t& operator()(
      mjz_class_operation_reporter_t& obj) const {
    return println("operator()(obj&)");
  }
  mjz_class_operation_reporter_t& operator()(
      const mjz_class_operation_reporter_t& obj) const {
    return println("operator()(const obj&)");
  }
  mjz_class_operation_reporter_t& operator()(
      mjz_class_operation_reporter_t&& obj) const {
    return println("operator()(obj&&)");
  }

  mjz_class_operation_reporter_t& operator()(int)const{ return println_oi("()"); }
  mjz_class_operation_reporter_t& operator++(int)const{ return println_oi("++"); }
  mjz_class_operation_reporter_t& operator++()const{ return println_o("++()"); }
  mjz_class_operation_reporter_t& operator--(int)const{ return println_oi("--"); }
  mjz_class_operation_reporter_t& operator--()const{ return println_o("--"); }

  mjz_class_operation_reporter_t& operator+(
      const mjz_class_operation_reporter_t& obj)const{
    return println_obj(obj, "+");
  }
  mjz_class_operation_reporter_t& operator-(
      const mjz_class_operation_reporter_t& obj)const{
    return println_obj(obj, "-");
  }

  mjz_class_operation_reporter_t& operator*(
      const mjz_class_operation_reporter_t& obj)const{
    return println_obj(obj, "*");
  }
  mjz_class_operation_reporter_t& operator/(
      const mjz_class_operation_reporter_t& obj)const{
    return println_obj(obj, "/");
  }
  mjz_class_operation_reporter_t& operator%(
      const mjz_class_operation_reporter_t& obj)const{
    return println_obj(obj, "%");
  }
  mjz_class_operation_reporter_t& operator>>(
      const mjz_class_operation_reporter_t& obj)const{
    return println_obj(obj, ">>");
  }
  mjz_class_operation_reporter_t& operator<<(
      const mjz_class_operation_reporter_t& obj)const{
    return println_obj(obj, "<<");
  }
  mjz_class_operation_reporter_t& operator&(
      const mjz_class_operation_reporter_t& obj)const{
    return println_obj(obj, "&");
  }
  mjz_class_operation_reporter_t& operator|(
      const mjz_class_operation_reporter_t& obj)const{
    return println_obj(obj, "|");
  }
  mjz_class_operation_reporter_t& operator^(
      const mjz_class_operation_reporter_t& obj)const{
    return println_obj(obj, "^");
  }
  

  mjz_class_operation_reporter_t& operator+=(
      const mjz_class_operation_reporter_t& obj)const{
    return println_obj(obj, "+=");
  }
  mjz_class_operation_reporter_t& operator-=(
      const mjz_class_operation_reporter_t& obj)const{
    return println_obj(obj, "-=");
  }
  mjz_class_operation_reporter_t& operator*=(
      const mjz_class_operation_reporter_t& obj)const{
    return println_obj(obj, "*=");
  }
  mjz_class_operation_reporter_t& operator/=(
      const mjz_class_operation_reporter_t& obj)const{
    return println_obj(obj, "/=");
  }
  mjz_class_operation_reporter_t& operator%=(
      const mjz_class_operation_reporter_t& obj)const{
    return println_obj(obj, "%=");
  }
  mjz_class_operation_reporter_t& operator>>=(
      const mjz_class_operation_reporter_t& obj)const{
    return println_obj(obj, ">>=");
  }
  mjz_class_operation_reporter_t& operator<<=(
      const mjz_class_operation_reporter_t& obj)const{
    return println_obj(obj, "<<=");
  }
  mjz_class_operation_reporter_t& operator&=(
      const mjz_class_operation_reporter_t& obj)const{
    return println_obj(obj, "&=");
  }
  mjz_class_operation_reporter_t& operator|=(
      const mjz_class_operation_reporter_t& obj)const{
    return println_obj(obj, "|=");
  }
  mjz_class_operation_reporter_t& operator^=(
      const mjz_class_operation_reporter_t& obj)const{
    return println_obj(obj, "^=");
  }
  mjz_class_operation_reporter_t& operator-() const { return println_o("-"); }
  mjz_class_operation_reporter_t& operator+() const { return println_o("+"); }
  mjz_class_operation_reporter_t* operator&()const{  // baaaaaaaaaaaaaaad practic
    println_o("&");
    return (mjz_class_operation_reporter_t*)this;
  }
  mjz_class_operation_reporter_t& operator*()const{
    println_o("*");
    return none();
  }
  mjz_class_operation_reporter_t* operator->()const{
    println_o("->");
    return (mjz_class_operation_reporter_t*)this;
  }
  template <typename T>
  auto operator->*(T p)const{
    println_o("->*");
    return this->*p;
  }
  inline operator bool()const{
    println_o(" bool ");
    return filler;
  }
  inline operator char()const{
    println_o(" char ");
    return filler;
  }

  inline operator int()const{
    println_o(" int ");
    return filler;
  }
  mjz_class_operation_reporter_t& operator!()const{ return println_o("!"); }
  mjz_class_operation_reporter_t& operator~()const{ return println_o("~"); }




  bool 
  operator&&(  // really bad practic to overload these dont do in any other
               // senario
      const mjz_class_operation_reporter_t& obj)const{
     println_obj(obj, "&&");
    return operator char() &&
          ((mjz_class_operation_reporter_t*)(&obj))->operator char();
  }
  bool
  operator||(  // really bad practic to overload these  dont do in any other
               // senario
      const mjz_class_operation_reporter_t& obj)const{
     println_obj(obj, "||");
    return operator char() || obj.operator char();
  }

  inline int64_t compare(const mjz_class_operation_reporter_t& obj)const{
    return (int64_t)((char*)UUID() - (char*)obj.UUID());
  }
  bool operator<=(const mjz_class_operation_reporter_t& obj)const{
    println_obj(obj, "<=");
    return compare(obj) <= 0;
  }
  bool operator>=(const mjz_class_operation_reporter_t& obj)const{
    println_obj(obj, ">=");

    return compare(obj) >= 0;
  }
  bool operator<(const mjz_class_operation_reporter_t& obj)const{
    println_obj(obj, "<");
    return compare(obj) < 0;
  }
  bool operator>(const mjz_class_operation_reporter_t& obj)const{
    println_obj(obj, ">");
    return compare(obj) > 0;
  }
  bool operator==(const mjz_class_operation_reporter_t& obj)const{
    println_obj(obj, "==");
    return compare(obj) == 0;
  }
  bool operator!=(const mjz_class_operation_reporter_t& obj)const{
    println_obj(obj, "!=");
    return compare(obj) != 0;
  }
  inline mjz_class_operation_reporter_t& none() const {
    return *((mjz_class_operation_reporter_t*)this);
  }

  
  inline operator mjz_class_operation_reporter_t&() const {
    return *((mjz_class_operation_reporter_t*)(this));
  }
  inline operator mjz_class_operation_reporter_t&()  {
    return * this ;
  }
  inline operator const mjz_class_operation_reporter_t&() const {
    return none();
  }
  inline operator const mjz_class_operation_reporter_t&() { return none(); }
   template <class the_Stream>
  friend the_Stream& operator<<(the_Stream& COUT, const mjz_class_operation_reporter_t& non_const_opr) {
    mjz_class_operation_reporter_t& opr =
        *(mjz_class_operation_reporter_t*)(&non_const_opr);  // i promise  that ther will be no changes in print functions but they are not const for being more usable
    opr.print_c_str(" ostream operator<< (obj) c:\'");
    opr.print_c_str_len_1(&opr.filler, 1);
    opr.print_c_str("\' ID : ");
    opr.print(opr.UUID());
    opr.print_c_str("\n");
    COUT << opr.filler;
        return COUT;
   }
   template <class the_Stream>
   friend the_Stream& operator>>(the_Stream& CIN,
                                 mjz_class_operation_reporter_t& opr) {
      
        opr.print_c_str(" istream operator>> (obj) c:\'");
        opr.print_c_str_len_1(&opr.filler, 1);
        opr.print_c_str("\' to \'");
        CIN >> opr.filler;
        opr.print_c_str_len_1(&opr.filler, 1);
        opr.print_c_str("\' ID : ");
        opr.print(opr.UUID());
        opr.print_c_str("\n");
        return CIN;
   }
};


template <class counter_class, class my_stream>
counter_class mjz_class_operation_reporter_t<counter_class, my_stream>::index{};
using operation_reporter = mjz_class_operation_reporter_t<size_t>;

namespace have_mjz_ard_removed {

using operation_reporter = mjz_class_operation_reporter_t<size_t>;
typedef vr_Scoped_speed_Timer vr_Scoped_speed_Timer;
typedef speed_Timer speed_Timer;
typedef Scoped_speed_Timer Scoped_speed_Timer;
}  // namespace have_mjz_ard_removed
}  // namespace mjz_ard
#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
