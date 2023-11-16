// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"
namespace mjz_ard{
 std::shared_ptr<std::map<std::string, timer_info>> Scoped_speed_Timer::g_map;
 Scoped_speed_Timer& Scoped_speed_Timer::operator()(const std::string& name,
              bool) {
 Stop();
 stoped = 0;
 m_Name = name;
 m_Timer.Reset();
 return *this;
 }
}





// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.
