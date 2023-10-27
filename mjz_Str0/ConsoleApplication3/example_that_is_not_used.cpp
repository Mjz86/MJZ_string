// ConsoleApplication3.cpp : This file contains the 'main' function. Program
// execution begins and ends there.
//

#include "mjzString.hpp"
static void print(const char* input) { std::cout << input; }
namespace dont_use {

using namespace have_mjz_ard_removed;

class mjz_Str_dir_test_class : public mjz_Str {
 public:
  virtual void* realloc(void* ptr, size_t new_size) override {
    void* retval_ = ::realloc(ptr, new_size);
    ::printf("\n\n mjz_Str_dir_test_class realloces in heap %d bytes ",
             (int)new_size);
    return retval_;
  }

  virtual void free(void* ptr) override {
    ::free(ptr);
    ::print("\n\n mjz_Str_dir_test_class frees \n\n");
    return;
  }
  void mjz_Str_dir_test_class_created() {
    ::print("\n\n mjz_Str_dir_test_class constructs \n\n");
  }
  mjz_Str_dir_test_class() { mjz_Str_dir_test_class_created(); }
  mjz_Str_dir_test_class(const mjz_Str& s) : mjz_Str(s) {
    mjz_Str_dir_test_class_created();
  }
  mjz_Str_dir_test_class(mjz_Str&& s) : mjz_Str(std::move(s)) {
    mjz_Str_dir_test_class_created();
  }
  mjz_Str_dir_test_class(const char* p) : mjz_Str(p) {
    mjz_Str_dir_test_class_created();
  }
  mjz_Str_dir_test_class(char c) : mjz_Str(c) {
    mjz_Str_dir_test_class_created();
  }
  mjz_Str_dir_test_class(unsigned char num) : mjz_Str(num) {
    mjz_Str_dir_test_class_created();
  }
  mjz_Str_dir_test_class(int num) : mjz_Str(num) {
    mjz_Str_dir_test_class_created();
  }
  mjz_Str_dir_test_class(unsigned int num) : mjz_Str(num) {
    mjz_Str_dir_test_class_created();
  }
  mjz_Str_dir_test_class(long num) : mjz_Str(num) {
    mjz_Str_dir_test_class_created();
  }
  mjz_Str_dir_test_class(unsigned long num) : mjz_Str(num) {
    mjz_Str_dir_test_class_created();
  }
  mjz_Str_dir_test_class(long long num) : mjz_Str(num) {
    mjz_Str_dir_test_class_created();
  }
  mjz_Str_dir_test_class(unsigned long long num) : mjz_Str(num) {
    mjz_Str_dir_test_class_created();
  }
  mjz_Str_dir_test_class(float num) : mjz_Str(num) {
    mjz_Str_dir_test_class_created();
  }
  mjz_Str_dir_test_class(double num) : mjz_Str(num) {
    mjz_Str_dir_test_class_created();
  }
  operator mjz_Str() const {
    ::print("\n\n mjz_Str_dir_test_class casted to mjz_Str && \n\n");
    return *this;
  }
  operator mjz_Str&() {
    ::print("\n\n mjz_Str_dir_test_class casted to mjz_Str & \n\n");
    return *this;
  }
  operator const mjz_Str&() const {
    ::print("\n\n mjz_Str_dir_test_class casted to mjz_Str C& \n\n");
    return *this;
  }
  virtual ~mjz_Str_dir_test_class() {
    mjz_Str_dir_test_class::free(
        m_buffer);  // don't need to but the vtable of the
    // mjz_Str_dir_test_class free gets destroyed when
    // ~mjz_Str() gets called so mjz_Str::free is called see
    //
    /* https://stackoverflow.com/questions/41732051/when-is-a-vtable-destroy-in-c

    Can I call a virtual function in the destructor of a base class?

    Calling virtual functions from destructors or constructors is a bad
    practice. See the standard (emphasis mine):

    12.7 Construction and destruction
    ....
    Member functions, including virtual functions (10.3),
    can be called during construction or destruction (12.6.2).
    When a virtual function is called directly or indirectly from a
    constructor or from a destructor, including during the construction or
    destruction of the class’s non-static data members, and the object to which
    the call applies is the object (call it x) under construction or
    destruction, the function called is the final overrider in the constructor’s
    or destructor’s class and not one overriding it in a more-derived class. If
    the virtual function call uses an explicit class member access (5.2.5) and
    the object expression refers to the complete object of x or one of that
    object’s base class subobjects but not x or one of its base class
    subobjects, the behavior is undefined.

    You can find this recommendation in many sources, including Scott Meyers'
    Effective C++: 55 Specific Ways to Improve Your Programs and Designs (Item
    9: Never call virtual functions during construction or destruction.)

    or Herb Sutter's
    C++ Coding Standards: 101 Rules, Guidelines, and Best Practices (49. Avoid
    calling virtual functions in constructors and destructors).
    */
    m_buffer = 0;  // prevent mjz_Str::free after free because of if(buffer)
    ::print("\n\n mjz_Str_dir_test_class destructs \n\n");
  }  // after this the ~mjz_Str() gets clled implicitly
  mjz_Str_dir_test_class(const mjz_Str_dir_test_class& other_) noexcept
      : mjz_Str(other_) {
    ::print("\n\n mjz_Str_dir_test_class copies and creates \n\n");
  }
  mjz_Str_dir_test_class(mjz_Str_dir_test_class&& other_) noexcept
      : mjz_Str(std::move(other_)) {
    ::print("\n\n mjz_Str_dir_test_class moves and creates\n\n");
  }
  mjz_Str_dir_test_class& operator=(const mjz_Str_dir_test_class& other_) {
    *((mjz_Str*)this) = (other_);
    ::print("\n\n mjz_Str_dir_test_class just copies \n\n");
    return *this;
  }
  mjz_Str_dir_test_class& operator=(mjz_Str_dir_test_class&& other_) noexcept {
    *((mjz_Str*)this) = (std::move(other_));
    ::print("\n\n mjz_Str_dir_test_class just moves \n\n");
    return *this;
  }
};

typedef mjz_Str String;
uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch())
      .count();
}

int main2() {
  uint64_t strtms = timeSinceEpochMillisec();
  int it_num = 10000;
  for (int k{}; k < it_num; k++) {
    String mystr(" hi mom \n");
    int myrownum = 100;
    int mycolnum = 400;
    mjz_Str output = mjz_Str::create_mjz_Str_2D_char_array(
        mycolnum, myrownum, '-');  // be careful using this without careing
    char** my2dar = (char**)output.C_str();

    for (int i{}; i < mycolnum; i++)
      for (int j{}; j < myrownum; j++)
        my2dar[i][j] = mjz_ard::GET_CHAR_from_int(
            (j + i * mycolnum) % 36, ((j + i * mycolnum) / 36) % 2);

    // for (int i{}; i < mycolnum; i++)
    // std::cout << '\n' << i << ".th col : \"" << my2dar[i] << "\" \n";
  }
  uint64_t strtms2 = timeSinceEpochMillisec();
  double ms_prop = (double)(strtms2 - strtms) / it_num;
  std::cout << "\nend in " << ms_prop << " ms per op \n ";
  for (int k{}; k < it_num; k++) {
    char* bfrnew = (new char[100 * 408]);  // TODO: V819
    // https://pvs-studio.com/en/docs/warnings/V819/
    // Decreased performance. Memory is allocated
    // and deleted multiple times inside the body of
    // the loop. Consider moving memory allocation
    // outside of the loop.
    for (long i{}; i < 100 * 408; i++) {
      bfrnew[i] = '0';
    }
    bfrnew[100 * 408 - 1] = 0;
    volatile auto bfrnew_str = " " + std::string((const char*)bfrnew) + " ";

    delete[] bfrnew;
  }
  double ms_prop2 = (double)(timeSinceEpochMillisec() - strtms2) / it_num;
  std::cout << "\nend in " << ms_prop2 << " ms per op \n ";

  std::cout << "\n class is " << (100 * ms_prop / ms_prop2)
            << "% slower per op \n ";

  return 0;
}
int main3() {
  {
    String mystr;
    mystr.change_reinterpret_char_char('\1');
    std::cin >> mystr;
    mystr.change_reinterpret_char_char('\\');
    mystr <<= *((mjz_Str*)(&mystr));
    std::cout << mystr;
  }
  return 0;
}

long loop2() {
  mjz_Str mystr(esp_random());
  return mystr()([&](mjz_Str* this_) -> long { return (long)this_->toLL(); });
}
long loop() {
  mjz_Str mystr;
  mystr.change_reinterpret_char_char('\1');
  std::cout << (mystr = "enter a sentence : \n");
  std::cin >> mystr();
  std::vector<std::pair<mjz_Str, size_t>> my_words(1);

  size_t word_count{};
  int64_t index_of_first_char_of_word{};
  int64_t index_of_first_space_after_previos{};
  bool DO_break{};
  while (!DO_break) {
    index_of_first_space_after_previos = [&]() -> int64_t {
      std::vector<int64_t> my_list_of_spacers = {
          mystr.indexOf(' ', index_of_first_char_of_word),
          mystr.indexOf('\n', index_of_first_char_of_word),
          mystr.indexOf(';', index_of_first_char_of_word),
          mystr.indexOf(',', index_of_first_char_of_word),
          mystr.indexOf('.', index_of_first_char_of_word)};

      auto result_it = std::min_element(
          my_list_of_spacers.begin(),  // chose the biggest
          my_list_of_spacers.end(), [](const auto& a, const auto& b) {
            constexpr int max_int = (uint32_t)(-1) >> 1;
            return (a == -1 ? max_int : a) < (b == -1 ? max_int : b);
          });
      return *result_it;
    }();
    if (index_of_first_space_after_previos == -1) {
      DO_break = 1;
      my_words.emplace(
          my_words.begin() + word_count,
          std::make_pair(mystr.substring(index_of_first_char_of_word),
                         word_count));
    } else {
      my_words.emplace(
          my_words.begin() + word_count,
          std::make_pair(mystr.substring(index_of_first_char_of_word,
                                         index_of_first_space_after_previos),
                         word_count));
    }
    index_of_first_char_of_word = index_of_first_space_after_previos + 1;
    if (my_words[word_count++].first.is_blank()) {
      my_words.erase(my_words.begin() + (--word_count));
    }
  }
  std::cout << "number of words is : " << word_count << "\n\r";
  for (auto& obj : my_words) {
    std::cout << obj.first;
    std::cout << "\n\r";
  }

  std::cout << "\n\r in words : \""
            << std::min_element(my_words.begin(), my_words.end(),
                                [](auto& a, auto& b) {
                                  return b.first.length() < a.first.length();
                                })
                   ->first
            << "\" is the longest \n heare is the revese order of them :\"";
  std::sort(my_words.begin(), my_words.end(),
            [](auto& a, auto& b) { return b.second < a.second; });
  for (auto& obj : my_words) {
    std::cout << obj.first;
    std::cout << " ";
  }
  std::sort(my_words.begin(), my_words.end(),
            [](auto& a, auto& b) { return a.second < b.second; });
  std::cout << "\" \n what word do you want to replace \n";
  std::cin >> mystr();
  std::cout << " replace with what \n";
  mjz_Str myrep_val;
  std::cin >> myrep_val;
  std::cout << " hear you go \n";
  for (auto& obj : my_words) {
    if (obj.first == mystr) obj.first = myrep_val;
    std::cout << obj.first << " ";
  }

  std::cout << "\n";

  return mystr()([](auto THis_) -> int {
    using namespace mjz_ard;
    mjz_Str& mystr = *THis_;
    mystr =
        "exiting enter some natural number to exit note that size of string \nis \n:"_m_str;
    mystr += sizeof(mjz_Str);
    mystr += " \n";
    mystr.change_reinterpret_char_char('\1');
    std::cout << mystr;
    std::cin >> mystr();
    mystr.change_reinterpret_char_char('\\');
    mystr <<= *((mjz_Str*)(&mystr));
    std::cout << mystr;
    return (long)mystr;  //|| !!main2();
  });
}

void setup() {}
long loop();
int main00() {
  setup();
  while (!loop())
    ;
  return 0;
}

int main79() {
  main2();
  if (mjz_Str("hello there i biult a cmd app press any word to start\n")(

          [&](mjz_Str* this_) -> bool {
            mjz_Str& obj = *this_;
            obj.change_reinterpret_char_char('\1');
            std::cout << obj;
            std::cin >> obj();
            obj.change_reinterpret_char_char('\\');
            obj <<= mjz_Str(obj);
            obj.replace(" ", 1, "", 0);
            obj.toLowerCase();
            if ((obj.is_blank())) return 0;

            if ((!!obj.toLL()) || (obj == "exit") || (obj == "e")) return 1;
            return 0;
          }))
    goto _end___;
  return main00();
_end___:
  return 0;
}
int main7() {
  mjz_Str mystr("hello there i biult a cmd app press any word to start\n");
  std::cout << "1. : \"" << mystr << "\"\n";
  std::cout << "2. : \"" << mystr.erase(5, 7) << "\"\n";
  mjz_Str mystr2;
  swap(mystr, mystr2);
  std::cout << "3. : \"" << mystr << "\"\n";
  std::cout << "4. : \"" << mystr2 << "\"\n";
  return 0;
}

const char* const _timer_sign = "_timer_";
void test_mstr_vs_sstr(
    const char* const c_str_small = "hi mom ;)",
    const char* const c_str_large = "allocation large string i am nesssery",
    size_t size_of_large = 37) {
  Scoped_speed_Timer timer("timer_");
  timer(_timer_sign);
  timer("mjz alloc");
  mjz_Str my_str(c_str_small);
  timer("mjz dealloc");
  my_str.~mjz_Str();
  timer("mjz alloc large");
  new (&my_str) mjz_Str(c_str_large, size_of_large);
  timer("mjz dealloc large");
  my_str.~mjz_Str();
  timer("mjz alloc base");
  new (&my_str) mjz_Str();
  timer("mjz dealloc base");
  my_str.~mjz_Str();
  timer("mjz alloc base");
  new (&my_str) mjz_Str();

  timer("std alloc");
  std::string my_strtd(c_str_small);
  timer("std dealloc");
  my_strtd.~basic_string();
  timer("std alloc large");
  new (&my_strtd) std::string(c_str_large, size_of_large);
  timer("std dealloc large");
  my_strtd.~basic_string();
  timer("std alloc base");
  new (&my_strtd) std::string();
  timer("std dealloc base");
  my_strtd.~basic_string();
  timer("std alloc base");
  new (&my_strtd) std::string();
  timer("mjz_str_view");
  using namespace mjz_ard::short_string_convestion_operators;
  char a_storage[sizeof(mjz_str_view)]{};
  new (a_storage) mjz_str_view("god code");
  timer("~mjz_str_view");
  reinterpret_cast<mjz_str_view*>(
      a_storage)  // TODO: V1032 https://pvs-studio.com/en/docs/warnings/V1032/
                  // The pointer 'a_storage' is cast to a more strictly aligned
                  // pointer type.
      ->~mjz_str_view();
  timer("timer_").Stop(timer_cmd::just_Stop);
}
const char* cstr_largeee =
    "allocation large string i am nesssery allocation large string i "
    "am nessseryallocation large string i am nesssery allocation large "
    "string i am nesssery";
int main786() {
  std::shared_ptr<std::map<std::string, timer_info>> map_ptr =
      std::make_shared<std::map<std::string, timer_info>>();
  {
    Scoped_speed_Timer time_r("total", false);
    time_r.set_global_map(map_ptr);
    for (size_t i{}; i < 1000; i++) {
      for (int j{}; j < 128; j++) {
        test_mstr_vs_sstr("hi mom ;)",
                          "allocation large string i am nesssery allocation "
                          "large string i am nessseryallocation large string i "
                          "am nesssery allocation large string i am nesssery",
                          j);
      }
    }
  }
  std::cout << " str view :" << sizeof(mjz_str_view) << " shredptr :"
            << sizeof(std::shared_ptr<mjz_ard::mjz_Str_DATA_storage_cls>)
            << " mjzstr :" << sizeof(mjz_Str) << " vs \n std::str "
            << sizeof(std::string) << " \n";
  std::cout << Scoped_speed_Timer::show_analysis(map_ptr, _timer_sign);
  std::this_thread::yield();
  std::cin.get();

  return main79();
}

void string_out(mjz_str_view input) { std::cout << input; }
int main976() {
  std::shared_ptr<std::map<std::string, timer_info>> map_ptr =
      std::make_shared<std::map<std::string, timer_info>>();
  Scoped_speed_Timer::set_global_map(map_ptr);
  char arr[1000][10]{};
  for (int i{}; i < 1000; i++) {
    Scoped_speed_Timer tm(_timer_sign);
    tm("str");
    using namespace mjz_ard;
    "abcdefg"_m_strv.copy(arr[i], 9);
    tm(_timer_sign);
  }
  std::cout << Scoped_speed_Timer::show_analysis(map_ptr, _timer_sign);
  std::cout << arr[0];
  return 0;
}
int main() {
  using namespace mjz_ard::short_string_convestion_operators;
  using namespace mjz_ard::short_string_names;
  using namespace mjz_ard;
  unsigned char dta[20]{};
  ("hi mom "_v + " hello "_v).getBytes(dta, 19);
  auto dt = mjz_str_view(dta);
  std::array<sv, 1000> ar;
  for (auto& obj : ar) {
    obj = dt;
  }
  dta[2] = 'F';
  for (const auto& obj : ar) {
    std::cout << obj;
  }
  return main786();
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
// 1. Use the Solution Explorer window to add/manage files
// 2. Use the Team Explorer window to connect to source control
// 3. Use the Output window to see build output and other messages
// 4. Use the Error List window to view errors
// 5. Go to Project > Add New Item to create new code files, or Project > Add
// Existing Item to add existing code files to the project
// 6. In the future, to open this project again, go to File > Open > Project
// and select the .sln file
int hash_demo() {
  using namespace mjz_ard::short_string_convestion_operators;
  using namespace have_mjz_ard_removed;

  std::array<uint8_t[32], 3> password_hash = {{
      {94,  136, 72,  152, 218, 40,  4,  113, 81, 208, 229,
       111, 141, 198, 41,  39,  115, 96, 61,  13, 106, 171,
       189, 214, 42,  17,  239, 114, 29, 21,  66, 216},  // hash of  "password"
      {1,   186, 71,  25,  200, 11, 111, 233, 17,  176, 145,
       167, 192, 81,  36,  182, 78, 238, 206, 150, 78,  9,
       192, 88,  239, 143, 152, 5,  218, 202, 84,  107},  // hash of "\n" //the
                                                          // string interpreter
                                                          // in operator<< will
                                                          // change a normal
                                                          // {'\\','n'} input to
                                                          // a {'\n'} mjz string
      {20,  141, 233, 197, 167, 164, 77,  25,  229, 108, 217,
       174, 26,  85,  75,  246, 120, 71,  175, 176, 197, 143,
       110, 18,  250, 41,  172, 125, 223, 202, 153, 64}  // hash of "p"
  }};

  mjz_str a;
  std::cin >> a;
#if 0
  auto hash_pair = a.hash_with_output();
   std::cout << hash_pair.second << "\n";
  auto& my_hash= hash_pair.first;
#else
  auto my_hash = a.hash();
#endif
  bool password_is_correct = false;
  for (uint8_t(&obj)[32]  : password_hash)
    password_is_correct |= (my_hash == obj);
 
  std::cout << (password_is_correct ? "correct" : "incorrect") << "\n";

  return password_is_correct;
}
}  // namespace dont_use
int main() {
  std::cout << "enter password :\n";
  if (!dont_use::hash_demo()) return 0;
  return dont_use::main976();
}