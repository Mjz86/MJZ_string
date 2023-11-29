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
template <class counter_class>
class mjz_class_operation_reporter_t {
  static counter_class index;
  template <typename... argT>
  mjz_class_operation_reporter_t& println(argT&&... args) {
    print_c_str(std::forward<argT>(args)...);
    std::cout << '\n';
    return *this;
  }
  template <typename... argT>
  mjz_class_operation_reporter_t& println_wf(
      mjz_class_operation_reporter_t& obj, argT&&... args) {
    print(std::forward<argT>(args)...);
    print_c_str(" with ID: ");
    std::cout << (UUID());
    print_c_str(" from ID:");
    std::cout << (obj.UUID());
    std::cout << '\n';

    return *this;
  }
  template <typename... argT>
  mjz_class_operation_reporter_t& println_w(argT&&... args) {
    print_c_str(std::forward<argT>(args)...);
    print_c_str(" with ID: ");
    std::cout << (UUID());
    std::cout << '\n';
    return *this;
  }
  template <typename... argT>
  mjz_class_operation_reporter_t& println_wi(const counter_class& index,
                                             argT&&... args) {
    print_c_str(std::forward<argT>(args)...);
    std::cout << index;
    print_c_str(" with ID: ");
    std::cout << (UUID());
    std::cout << '\n';
    return *this;
  }
  template <typename... argT>
  mjz_class_operation_reporter_t& printsv(argT&&... args) {
    std::initializer_list<std::string_view> list = {
        std::forward<argT>(args)...};
    for (auto& s : list) std::cout << s;
    return *this;
  }
  template <typename... argT>
  mjz_class_operation_reporter_t& print_c_str(argT&&... args) {
    std::initializer_list<const char*> list = {std::forward<argT>(args)...};
    for (auto& s : list) std::cout << s;
    return *this;
  }
  template <typename... argT>
  mjz_class_operation_reporter_t& print(argT&&... args) {
    auto list =
        std::initializer_list<std::function<void(void)>>{[&](void) -> void {
          std::cout << std::forward<argT>(args);
          return;
        }...};  // do all tasks in thr rigth order
    for (auto& f : list) f();
    return *this;
  }
  template <typename T>
  mjz_class_operation_reporter_t& println_o(T&& arg) {
    println_w("operator", std::forward<T>(arg), "()");
    return *this;
  }
  template <typename T>
  mjz_class_operation_reporter_t& println_oi(T&& arg) {
    println_w("operator", std::forward<T>(arg), "(int)");
    return *this;
  }
  template <typename T>
  mjz_class_operation_reporter_t& println_obj(T&& arg) {  // oobj
    println_w("operator", std::forward<T>(arg), "(const obj&)");
    return *this;
  }

 public:
  char filler = '|';
  inline const void* UUID() const { return this; };
  mjz_class_operation_reporter_t() { println_wi(index++, " created : "); }
  mjz_class_operation_reporter_t(int i) : filler(i) {
    println_wi(index++, " created with int : ");
  }
  ~mjz_class_operation_reporter_t() { println_wi(--index, " destroyed : "); }
  mjz_class_operation_reporter_t(mjz_class_operation_reporter_t&& obj) {
    index++;
    println_wf(obj, " move constructed ");
  }
  mjz_class_operation_reporter_t& operator=(
      mjz_class_operation_reporter_t&& obj) {
    println_wf(obj, " moved to me  ");
    return *this;
  }
  mjz_class_operation_reporter_t(const mjz_class_operation_reporter_t& obj) {
    index++;
    println_wf(obj, " copy constructed ");
  }
  mjz_class_operation_reporter_t& operator=(
      const mjz_class_operation_reporter_t& obj) {
    println_wf(obj, " copied to me   ");
    return *this;
  }
  mjz_class_operation_reporter_t& operator()() { return println_o("()"); }
  mjz_class_operation_reporter_t& operator()(int) { return println_oi("()"); }
  mjz_class_operation_reporter_t& operator++(int) { return println_oi("++"); }
  mjz_class_operation_reporter_t& operator++() { return println_o("++()"); }
  mjz_class_operation_reporter_t& operator--(int) { return println_oi("--"); }
  mjz_class_operation_reporter_t& operator--() { return println_o("--"); }

  mjz_class_operation_reporter_t& operator<=(
      const mjz_class_operation_reporter_t&) {
    return println_obj("<=");
  }
  mjz_class_operation_reporter_t& operator>=(
      const mjz_class_operation_reporter_t&) {
    return println_obj(">=");
  }
  mjz_class_operation_reporter_t& operator<(
      const mjz_class_operation_reporter_t&) {
    return println_obj("<");
  }
  mjz_class_operation_reporter_t& operator>(
      const mjz_class_operation_reporter_t&) {
    return println_obj(">");
  }
  mjz_class_operation_reporter_t& operator==(
      const mjz_class_operation_reporter_t&) {
    return println_obj("==");
  }
  mjz_class_operation_reporter_t& operator!=(
      const mjz_class_operation_reporter_t&) {
    return println_obj("!=");
  }
  mjz_class_operation_reporter_t& operator+(
      const mjz_class_operation_reporter_t&) {
    return println_obj("+");
  }
  mjz_class_operation_reporter_t& operator-(
      const mjz_class_operation_reporter_t&) {
    return println_obj("-");
  }
  mjz_class_operation_reporter_t& operator*(
      const mjz_class_operation_reporter_t&) {
    return println_obj("*");
  }
  mjz_class_operation_reporter_t& operator/(
      const mjz_class_operation_reporter_t&) {
    return println_obj("/");
  }
  mjz_class_operation_reporter_t& operator>>(
      const mjz_class_operation_reporter_t&) {
    return println_obj(">>");
  }
  mjz_class_operation_reporter_t& operator<<(
      const mjz_class_operation_reporter_t&) {
    return println_obj("<<");
  }
  mjz_class_operation_reporter_t& operator&(
      const mjz_class_operation_reporter_t&) {
    return println_obj("&");
  }
  mjz_class_operation_reporter_t& operator|(
      const mjz_class_operation_reporter_t&) {
    return println_obj("|");
  }
  mjz_class_operation_reporter_t& operator^(
      const mjz_class_operation_reporter_t&) {
    return println_obj("^");
  }
  mjz_class_operation_reporter_t&
  operator&&(  // really bad practic to overload these dont do in any other
               // senario
      const mjz_class_operation_reporter_t&) {
    return println_obj("&&");
  }
  mjz_class_operation_reporter_t&
  operator||(  // really bad practic to overload these  dont do in any other
               // senario
      const mjz_class_operation_reporter_t&) {
    return println_obj("||");
  }

  mjz_class_operation_reporter_t& operator+=(
      const mjz_class_operation_reporter_t&) {
    return println_obj("+=");
  }
  mjz_class_operation_reporter_t& operator-=(
      const mjz_class_operation_reporter_t&) {
    return println_obj("-=");
  }
  mjz_class_operation_reporter_t& operator*=(
      const mjz_class_operation_reporter_t&) {
    return println_obj("*=");
  }
  mjz_class_operation_reporter_t& operator/=(
      const mjz_class_operation_reporter_t&) {
    return println_obj("/=");
  }
  mjz_class_operation_reporter_t& operator>>=(
      const mjz_class_operation_reporter_t&) {
    return println_obj(">>=");
  }
  mjz_class_operation_reporter_t& operator<<=(
      const mjz_class_operation_reporter_t&) {
    return println_obj("<<=");
  }
  mjz_class_operation_reporter_t& operator&=(
      const mjz_class_operation_reporter_t&) {
    return println_obj("&=");
  }
  mjz_class_operation_reporter_t& operator|=(
      const mjz_class_operation_reporter_t&) {
    return println_obj("|=");
  }
  mjz_class_operation_reporter_t& operator^=(
      const mjz_class_operation_reporter_t&) {
    return println_obj("^=");
  }

  mjz_class_operation_reporter_t& operator->() { return println_o("->"); }
  template <typename T>
  auto operator->*(T p) {
    println_o("->*");
    return this->*p;
  }
  inline operator bool() {
    println_o(" bool ");
    return filler;
  }

  mjz_class_operation_reporter_t& operator!() { return println_o("!"); }
  mjz_class_operation_reporter_t& operator~() { return println_o("~"); }
};
template <class counter_class>
counter_class mjz_class_operation_reporter_t<counter_class>::index{};
using operation_reporter = mjz_class_operation_reporter_t<size_t>;

namespace have_mjz_ard_removed {

using operation_reporter = mjz_class_operation_reporter_t<size_t>;
typedef vr_Scoped_speed_Timer vr_Scoped_speed_Timer;
typedef speed_Timer speed_Timer;
typedef Scoped_speed_Timer Scoped_speed_Timer;
}  // namespace have_mjz_ard_removed
}  // namespace mjz_ard
#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
