//std::is_invocable_v
#include "my_main.h"
namespace mjz_ard {};  // namespace mjz_ard
namespace test {
static bool fn_mjz(mjzt::calee_ret<mjzt::operation_reporter> ret,
                   int condition) noexcept {
  CE_NE_RETURN_IF0(ret);
  switch (condition) {
    case 0:
      CE_NE_RETURN_EMPLACE(ret, "i am returned {0}");
      break;
    case 1:
      try {  // safer for outside even if emplace is noexcept
        ret->emplace(" initialized {1}");
      } catch (...) {
      }
      break;
    case 2:
      CE_NE_RETURN_EMPLACE(ret, "i am returned {2}");
      break;
    default:
      try {
        ret->emplace('D');
        ~ret();
      } catch (...) {
      }
      break;
  }
  CE_NE_RETURN_WITH(ret);
}
static void run_ret() {
  int i{};
  while (i != 3) {
    mjz::iostream::println("give an integer {3 to exit}");
    char buf[4]{};
    std::cin.getline(buf, 3);
    {
      for (auto c : buf) {
        if (c == 0) break;
        if ('0' <= c && c <= '9') continue;
        i = 3;
      }
      if (i == 3 || !*buf) {
        mjz::iostream::println(" :( ");
        break;
      }
      i = mjzt::mjz_str(buf, 3).toLL();
    }

    mjzt::caler_ret<mjzt::operation_reporter> ret_;
    if (!fn_mjz(CR_CALL_IF(i != 3, ret_), i)) {
      mjz::iostream::println("no return {!(1 || 0 || 3)}");
      continue;
    }
    mjzt::operation_reporter& ret = ret_;
    ret++;
  }
}

bool get_float(mjzt::calee_ret<float> ret) {
  USE_MJZ_NS();
  CE_RETURN_IF0(ret);
  float flt{};
  println("enter float: ");
  scan(flt);
  if (flt != NAN) {
    CE_RETURN_EMPLACE(ret, flt);
  }
  ret->emplace(3);
  CE_RETURN_WITH(ret);
}
const static mjz::BYTE hash[] = {94,  -120, 72,  -104, -38, 40,  4,  113, 81, -48, -27,
                          111, -115, -58, 41,   39,  115, 96, 61,  13, 106, -85,
                          -67, -42,  42,  17,   -17, 114, 29, 21,  66, -40};

#define MK_UP_PR(U, P) std::make_pair<mjzt::mstrv, const mjz::BYTE* const>(U, P)
std::map<mjzt::mstrv, const mjz::BYTE* const> user_login{
    MK_UP_PR("user", hash)};

bool get_user_password(mjzt::calee_ret<mjzt::mjz_str> ret) {
  USE_MJZ_NS();
  CE_RETURN_IF0(ret);
  uint32_t i{};
  println(" user list: ");
  for (auto& [k,v] : user_login) println(++i, " :  \"", k, '"');
  println("enter user: ");
  scanln(*+*ret);
  ret->o().toLowerCase();
  
  println("enter password: ");
  
   
    mjz_str ps;
    scanln(ps);
 auto   psho= ps.mjz_hash();
    optional_ptr <std::remove_reference_t< decltype(user_login.at(**ret)) > * > p;
    try {
   p= user_login.at(**ret);
    } catch (...) {
    }
    if (!!p)
 {
    if (*p != psho)
    ~ret;
    CE_RETURN_WITH(ret);
 }
 ~ret;
  CE_RETURN_WITH(ret);
}

};  // namespace test

int my_main::main(int argc, const char* const* const argv) {
  USE_MJZ_NS();
  using namespace test;
  bool rn{1};
 // println("password"_s.mjz_hash());
  if (!get_user_password(CR_NO_RETURN(caler_ret<mjz_str>()))) {
    println("SORRY :( ");
    return 0;
  }
  {
    caler_ret<float> ret;

    if (get_float(CR_CALL_IF(!(std::rand() % 10000), ret))) {
      ignore();
      if (f_EQ(*ret, 3, 0.1)) {
        rn = 0;
      }
    }
  }
  if (rn) run_ret();
  return 0;
}
