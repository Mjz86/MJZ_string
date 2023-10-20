// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"
std::shared_ptr<std::map<std::string, timer_info>> Scoped_speed_Timer::g_map;
// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.
