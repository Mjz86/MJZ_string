#pragma once

#include <chrono>
#include <cstdint>
#include <iostream>

#include "mjzString_helper.hpp"

class speed_Timer {
  public:
    speed_Timer() { Reset(); }

    void Reset() { m_Start = std::chrono::high_resolution_clock::now(); }

    long double Elapsednano() {
      return ( long double )std::chrono::duration_cast<std::chrono::nanoseconds>(
               std::chrono::high_resolution_clock::now() - m_Start )
             .count(); }
    long double Elapsed() { return Elapsednano() * 0.001f * 0.001f * 0.001f; }

    long double ElapsedMillis() { return Elapsednano() / 1000.0f / 1000.0f; }

  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_Start; };
struct timer_info {
  long double Time_ns;
  long double atempt_num; };
inline bool operator<( timer_info rhs, timer_info lhs ) {
  return ( rhs.Time_ns / rhs.atempt_num ) < ( lhs.Time_ns / lhs.atempt_num ); }
enum timer_cmd : uint8_t {
  NONE = 0,
  Force_log = MJZ_frm_stack_ovf_BIT( 0 ),
  NO_Stop = MJZ_frm_stack_ovf_BIT( 1 ),
  just_Stop = MJZ_frm_stack_ovf_BIT( 2 ), };
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
        goto return_;
      }

      if (m_uomp) {
        timer_info& ti_ = (*m_uomp)[m_Name];
        ti_.atempt_num++;
        ti_.Time_ns += time;
      } else {
        std::cout << "[TIMER] " << m_Name << " - " << time << "ns\n";
      }

   return_:

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
    bool stoped{ };
    std::string m_Name;
    static std::shared_ptr<std::map<std::string, timer_info>> g_map;
    std::shared_ptr<std::map<std::string, timer_info>> m_uomp;
    speed_Timer m_Timer; };
class vr_Scoped_speed_Timer : public Scoped_speed_Timer {
    vr_Scoped_speed_Timer( const std::string & name ) : Scoped_speed_Timer( name ) {}
    vr_Scoped_speed_Timer( const Scoped_speed_Timer & name )
      : Scoped_speed_Timer( name ) {}
    vr_Scoped_speed_Timer( Scoped_speed_Timer && name )
      : Scoped_speed_Timer( std::move( name ) ) {}
    vr_Scoped_speed_Timer( const vr_Scoped_speed_Timer & name )
      : Scoped_speed_Timer( name ) {}
    vr_Scoped_speed_Timer( vr_Scoped_speed_Timer && name ) noexcept
      : Scoped_speed_Timer( std::move( name ) ) {}
    vr_Scoped_speed_Timer & operator=( const vr_Scoped_speed_Timer & s_tmr ) {
      *( ( Scoped_speed_Timer * )this ) = s_tmr;
      return *this; }
    vr_Scoped_speed_Timer & operator=( vr_Scoped_speed_Timer && s_tmr ) noexcept {
      *( ( Scoped_speed_Timer * )this ) = std::move( s_tmr );
      return *this; }
    vr_Scoped_speed_Timer & operator=( const Scoped_speed_Timer & s_tmr ) {
      *( ( Scoped_speed_Timer * )this ) = s_tmr;
      return *this; }
    vr_Scoped_speed_Timer & operator=( Scoped_speed_Timer && s_tmr ) {
      *( ( Scoped_speed_Timer * )this ) = std::move( s_tmr );
      return *this; }

    virtual ~vr_Scoped_speed_Timer() = default; };

#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
