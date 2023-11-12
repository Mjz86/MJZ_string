#include "mjzString.hpp"
static void print(const char *input) { std::cout << input; }
namespace dont_use {

using namespace have_mjz_ard_removed;

class mjz_Str_dir_test_class : public mjz_Str {
public:
  virtual void *realloc(void *ptr, size_t new_size) override {
    void *retval_ = ::realloc(ptr, new_size);
    ::printf("\n\n mjz_Str_dir_test_class realloces in heap %d bytes ",
             (int)new_size);
    return retval_;
  }

  virtual void free(void *ptr) override {
    ::free(ptr);
    ::print("\n\n mjz_Str_dir_test_class frees \n\n");
    return;
  }
  void mjz_Str_dir_test_class_created() {
    ::print("\n\n mjz_Str_dir_test_class constructs \n\n");
  }
  mjz_Str_dir_test_class() { mjz_Str_dir_test_class_created(); }
  mjz_Str_dir_test_class(const mjz_Str &s) : mjz_Str(s) {
    mjz_Str_dir_test_class_created();
  }
  mjz_Str_dir_test_class(mjz_Str &&s) : mjz_Str(std::move(s)) {
    mjz_Str_dir_test_class_created();
  }
  mjz_Str_dir_test_class(const char *p) : mjz_Str(p) {
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
  operator mjz_Str &() {
    ::print("\n\n mjz_Str_dir_test_class casted to mjz_Str & \n\n");
    return *this;
  }
  operator const mjz_Str &() const {
    ::print("\n\n mjz_Str_dir_test_class casted to mjz_Str C& \n\n");
    return *this;
  }
  virtual ~mjz_Str_dir_test_class() {
    mjz_Str_dir_test_class::free(
        m_buffer); // don't need to but the vtable of the
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
      destruction of the class s non-static data members, and the object to
      which the call applies is the object (call it x) under construction or
      destruction, the function called is the final overrider in the
      constructor s or destructor s class and not one overriding it in a
      more-derived class. If the virtual function call uses an explicit class
      member access (5.2.5) and the object expression refers to the complete
      object of x or one of that object s base class sub objects but not x or
      one of its base class sub objects, the behavior is undefined.

      You can find this recommendation in many sources, including Scott Meyers'
      Effective C++: 55 Specific Ways to Improve Your Programs and Designs (Item
      9: Never call virtual functions during construction or destruction.)

      or Herb Sutter's
      C++ Coding Standards: 101 Rules, Guidelines, and Best Practices (49. Avoid
      calling virtual functions in constructors and destructors).
    */
    m_buffer = 0; // prevent mjz_Str::free after free because of if(buffer)
    ::print("\n\n mjz_Str_dir_test_class destructs \n\n");
  } // after this the ~mjz_Str() gets clled implicitly
  mjz_Str_dir_test_class(const mjz_Str_dir_test_class &other_) noexcept
      : mjz_Str(other_) {
    ::print("\n\n mjz_Str_dir_test_class copies and creates \n\n");
  }
  mjz_Str_dir_test_class(mjz_Str_dir_test_class &&other_) noexcept
      : mjz_Str(std::move(other_)) {
    ::print("\n\n mjz_Str_dir_test_class moves and creates\n\n");
  }
  mjz_Str_dir_test_class &operator=(const mjz_Str_dir_test_class &other_) {
    *((mjz_Str *)this) = (other_);
    ::print("\n\n mjz_Str_dir_test_class just copies \n\n");
    return *this;
  }
  mjz_Str_dir_test_class &operator=(mjz_Str_dir_test_class &&other_) noexcept {
    *((mjz_Str *)this) = (std::move(other_));
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
 
int main3() {
  {
    String mystr;
    mystr.change_reinterpret_char_char('\1');
    std::cin >> mystr;
    mystr.change_reinterpret_char_char('\\');
    mystr <<= *((mjz_Str *)(&mystr));
    std::cout << mystr;
  }
  return 0;
}

long loop2() {
  mjz_Str mystr(esp_random());
  return mystr()([&](mjz_Str &obj) -> long { return (long)obj.toLL(); });
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
          my_list_of_spacers.begin(), // chose the biggest
          my_list_of_spacers.end(), [](const auto &a, const auto &b) {
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

  for (auto &obj : my_words) {
    std::cout << obj.first;
    std::cout << "\n\r";
  }

  std::cout << "\n\r in words : \""
            << std::min_element(my_words.begin(), my_words.end(),
                                [](auto &a, auto &b) {
                                  return b.first.length() < a.first.length();
                                })
                   ->first
            << "\" is the longest \n heare is the revese order of them :\"";
  std::sort(my_words.begin(), my_words.end(),
            [](auto &a, auto &b) { return b.second < a.second; });

  for (auto &obj : my_words) {
    std::cout << obj.first;
    std::cout << " ";
  }

  std::sort(my_words.begin(), my_words.end(),
            [](auto &a, auto &b) { return a.second < b.second; });
  std::cout << "\" \n what word do you want to replace \n";
  std::cin >> mystr();
  std::cout << " replace with what \n";
  mjz_Str myrep_val;
  std::cin >> myrep_val;
  std::cout << " hear you go \n";

  for (auto &obj : my_words) {
    if (obj.first == mystr) {
      obj.first = myrep_val;
    }

    std::cout << obj.first << " ";
  }

  std::cout << "\n";
  return mystr()([](auto THis_) -> int {
    using namespace mjz_ard;
    mjz_Str &mystr = *THis_;
    mystr =
        "exiting enter some natural number to exit note that size of string \nis \n:"_m_str;
    mystr += sizeof(mjz_Str);
    mystr += " \n";
    mystr.change_reinterpret_char_char('\1');
    std::cout << mystr;
    std::cin >> mystr();
    mystr.change_reinterpret_char_char('\\');
    mystr <<= *((mjz_Str *)(&mystr));
    std::cout << mystr;
    return (long)mystr; //|| !!main2();
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

  mjz_Str("hello there i biult a cmd app press any word to start\n")(
      [&](mjz_Str &obj) -> bool {
        obj.change_reinterpret_char_char('\1');
        std::cout << obj;
        std::cin >> obj();
        obj.change_reinterpret_char_char('\\');
        obj <<= mjz_Str(obj);
        obj.find_and_replace(" ", 1, "", 0);
        obj.toLowerCase();

        if ((obj.is_blank()))
          return 0;

        if ((!!obj.toLL()) || (obj == "exit") || (obj == "e"))
          return 1;
        return 0;
      });

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

const char *const _timer_sign = "_timer_";
void test_mstr_vs_sstr(
    const char *const c_str_small = "hi mom ;)",
    const char *const c_str_large = "allocation large string i am nesssery",
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
  mjz_ard::heap_obj_warper<mjz_str_view> view_storage;
  view_storage.data_init("god code");
  timer("~mjz_str_view");
  view_storage.data_de_init();
  timer("timer_").Stop(timer_cmd::just_Stop);
}
const char *cstr_largeee =
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
  std::cout << " str view :" << sizeof(mjz_str_view) << " shred_ptr :"
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

  for (auto &obj : ar) {
    obj = dt;
  }

  dta[2] = 'F';

  for (const auto &obj : ar) {
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
  std::array<uint8_t[32], 4> password_hash = {{
      {94,  136, 72,  152, 218, 40,  4,  113, 81, 208, 229,
       111, 141, 198, 41,  39,  115, 96, 61,  13, 106, 171,
       189, 214, 42,  17,  239, 114, 29, 21,  66, 216}, // hash of  "password"
      {1,   186, 71,  25,  200, 11, 111, 233, 17,  176, 145,
       167, 192, 81,  36,  182, 78, 238, 206, 150, 78,  9,
       192, 88,  239, 143, 152, 5,  218, 202, 84,  107}, // hash of "\n" //the
      // string interpreter
      // in operator<< will
      // change a normal
      // {'\\','n'} input to
      // a {'\n'} mjz string
      {20,  141, 233, 197, 167, 164, 77,  25,  229, 108, 217,
       174, 26,  85,  75,  246, 120, 71,  175, 176, 197, 143,
       110, 18,  250, 41,  172, 125, 223, 202, 153, 64} // hash of "p"
      ,
      {175, 219, 195, 16,  252, 20,  32,  118, 220, 122, 102,
       106, 27,  138, 16,  25,  232, 87,  120, 105, 31,  158,
       241, 18,  98,  231, 29,  99,  173, 186, 108, 169} // hash of "toor_pass"
  }};
  mjz_str a;
  std::cin >> a;
  auto my_hash = a.hash();
  std::cout << my_hash.to_string();
  bool password_is_correct = false;

  for (uint8_t(&obj)[32] : password_hash) {
    password_is_correct |= (my_hash == obj);
  }

  std::cout << (password_is_correct ? "correct" : "incorrect") << "\n"_s;
  return password_is_correct;
}
bool should_wait_for_input{1};
bool get_password_thread() {
  std::cout << "enter password :\n";
  using namespace std::chrono_literals;
  std::chrono::seconds seconds_wait{0s};

  while (!dont_use::hash_demo()) {
    std::cout << "please try again after " << ++seconds_wait << " seconds \n";
    std::this_thread::sleep_for(seconds_wait);

    if (seconds_wait > 3s) {
      return 0;
    }
  }

  return 1;
}
int main86469() {
  mjz_ard::mjz_Str str;
  std::thread get_password(get_password_thread);
  get_password.join();
  dont_use::main976();
  str("0123456789");
  //  auto sv1=  str.substr_view_beg_n((size_t)1,(size_t)8);
  // auto sv2 = str.substr_view_beg_n( -4LL, (size_t)8);
  auto sv3 = str.substr_view_beg_n(-9LL, str.length());
  std::cout << "\n\n"
            << sv3 // << sv1 << sv2 << mjz_ard::mjz_Str(sv1) <<
                   // mjz_ard::mjz_Str(sv2)
            << "\n\n";
  using namespace mjz_ard::short_string_convestion_operators;
  using namespace mjz_ard::short_string_names;
  using namespace mjz_ard;
  std::map<mjz_Str, mjz_Str> map_str;

  for (int i{}; i < 1000; i++) {
    map_str[mjz_Str(esp_random())] = esp_random();
  }

  for (auto &[k, v] : map_str) {
    std::cout << k << " : " << v << " \n";
  }

  return 0;
}

int maint() {
  using namespace mjz_ard;
  using namespace mjz_ard::short_string_convestion_operators;
  using namespace mjz_ard::short_string_names;

  std::vector<std::pair<std::pair<double, double>, std::pair<double, double>>>
      num_a;
  for (double x{0}; x < 10; x += 1) { //= 0.01
    double a = mjz_ard::static_str_algo::sqrt(x);
    double b = sqrt(x);
    double del = abs(a - b);

    num_a.push_back({{x, del}, {a, b}});
  }
  std::sort(num_a.begin(), num_a.end(),
            [](auto a, auto b) { return b.first.second < a.first.second; });

  for (auto &[kv, ab] : num_a) {
    auto &[k, v] = kv;
    auto &[a, b] = ab;
    std::cout << k << " : " << v << "=" << a << "-" << b << "\n";
  }
  return 0;
}
int maini() {
  mjz_ard::mjz_Str buffer;
  mjz_ard::mjz_Str str;
  char buf[1024]{};
  {
    Scoped_speed_Timer tm("tm");
    for (uint32_t i{}; i < 100000; i++) {
      buffer += (str = std::rand()).hash().copy_to_c_string(buf, 1024);
    }
  }
  std::cin.get();
  std::cout << buffer;

  return 0;
}

} // namespace dont_use

namespace my_failed_neural_net {
inline float activation_function(float x) {
  return mjz_ard::static_str_algo::LeakyReLU(x);
}
inline float activation_function_dr(float x) {
  return mjz_ard::static_str_algo::LeakyReLUDer(x);
}
inline std::vector<float> activation_function(std::vector<float> sum) {
  for (auto &x : sum)
    x = mjz_ard::static_str_algo::LeakyReLU(x);
  return sum;
}
inline std::vector<float> activation_function_dr(std::vector<float> sum) {
  for (auto &x : sum)
    x = mjz_ard::static_str_algo::LeakyReLUDer(x);
  return sum;
}

static constexpr float learning_rate{0.01};
inline float calculate_output_error(float output, float expected_output) {
  return expected_output - output;
}

inline float calculate_derivitive_at_sum(float sum) {
  return activation_function_dr(sum);
}
inline float calculate_error_derivitive_multiplier(float derivitive_at_sum,
                                                   float output_error) {
  return learning_rate * derivitive_at_sum * output_error;
}

inline float calculate_delta_weight(float error_derivitive_multiplier,
                                    float input_at_i) {
  return error_derivitive_multiplier * input_at_i;
}
inline float calculate_expected_input(float input_at_i, float delta_weight,
                                      float err) {
  return delta_weight / err + input_at_i;
}
float calculate_sum(uint32_t input_count, const std::vector<float> &inputs,
                    const std::vector<float> &weights) {
  double sum{weights[0]};
  for (uint32_t i{}; i < input_count;) {
    double input_at_i = inputs[i];
    input_at_i *= weights[++i];
    sum += input_at_i;
  }
  return sum;
}
std::vector<float>
calculate_sum(uint32_t input_count, const std::vector<float> &inputs,
              const std::vector<std::vector<float>> &weights) {
  size_t NumberOf_neurons = weights.size();
  std::vector<float> sum(NumberOf_neurons);
  for (uint32_t i{}; i < NumberOf_neurons; i++) {
    sum[i] = calculate_sum(input_count, inputs, weights[i]);
  }
  return sum;
}
inline void change_weights(uint32_t input_count,
                           std::vector<float> &expected_inputs,
                           const std::vector<float> &inputs,
                           std::vector<float> &weights, float sum_of_in_times_w,
                           float pervious_output, float expected_output) {
  float output_error = calculate_output_error(pervious_output, expected_output);
  float derivitive_at_sum = calculate_derivitive_at_sum(sum_of_in_times_w);
  float error_derivitive_multiplier =
      calculate_error_derivitive_multiplier(derivitive_at_sum, output_error);

  weights[0] += calculate_delta_weight(error_derivitive_multiplier, 1);

  for (uint32_t i{}; i < input_count;) {
    float delta_weight =
        calculate_delta_weight(error_derivitive_multiplier, inputs[i]);
    expected_inputs[i] = calculate_expected_input(inputs[i], delta_weight,
                                                  error_derivitive_multiplier);
    weights[++i] += delta_weight;
  }
}

inline std::vector<float> change_weights_in_layer(
    uint32_t input_count, uint32_t layer_n_count,
    const std::vector<float> &inputs, std::vector<std::vector<float>> &weights,
   const std::vector<float>& sum_of_in_times_w, const std::vector<float>& pervious_output,
    const std::vector<float>& expected_output) {
  std::vector<float> ei(input_count);
  for (uint32_t i{}; i < layer_n_count; i++) {
    change_weights(input_count, ei, inputs, weights[i], sum_of_in_times_w[i],
                   pervious_output[i], expected_output[i]);
  }
  for (auto &e : ei)
    e /= layer_n_count;
  return ei;
}
void mjz_print(const std::vector<float> &v) {
  auto end = v.end();
  auto it = v.begin();
  std::cout << "{";
  std::cout << *it;
  ++it;
  for (; it < end; it++) {
    std::cout << "," << *it;
  }
  std::cout << "}";
}

void mjz_print(const std::vector<std::vector<float>> &v) {
  auto end = v.end();
  auto it = v.begin();
  std::cout << "{";
  mjz_print(*it);
  ++it;
  for (; it < end; it++) {
    std::cout << ",";
    mjz_print(*it);
  }
  std::cout << "}";
}
void mjz_print(const std::vector<std::vector<std::vector<float>>> &v) {
  auto end = v.end();
  auto it = v.begin();
  std::cout << "{";
  mjz_print(*it);
  ++it;
  for (; it < end; it++) {
    std::cout << ",\n";
    mjz_print(*it);
  }
  std::cout << "}";
}
std::vector<float> calculate_one_layer(std::vector<std::vector<float>> &weights,
                                       const std::vector<float>& inputs,
                                       const std::vector<float>& expected_outputs,
                                       std::vector<float> &sum,
                                       std::vector<float> &outputs,
                                       double &error_cost) {
  for (uint32_t j{}; j < inputs.size(); j++) {
    auto ex_output = expected_outputs[j];
    error_cost += pow(outputs[j] - ex_output, 2);
  }
  std::vector<float> expected_inputs =
      change_weights_in_layer(inputs.size(), weights.size(), inputs, weights,
                              sum, outputs, expected_outputs);
  return expected_inputs;
}
std::vector<float>
forward_propgate_one_layer(std::vector<std::vector<float>> &weights,
                           const std::vector<float> &inputs,
                           std::vector<float> &sum) {
  sum = calculate_sum(inputs.size(), inputs, weights);
  std::vector<float> ret = sum;
  for (auto &x : ret)
    x = mjz_ard::static_str_algo::LeakyReLU(x);
  return ret;
}

std::vector<float>
pridict(std::vector<std::vector<std::vector<float>>> &weights,
        const std::vector<float>& inputs) {
  std::vector<float> output;
  std::vector<float> buffer;
  output = forward_propgate_one_layer(weights[0], inputs, buffer);
  auto last = weights.size();
  for (uint32_t i{1}; i < last; i++) {
    output = forward_propgate_one_layer(weights[i], output, buffer);
  }
  return output;
}
double back_propagate(std::vector<std::vector<std::vector<float>>> &weights,
                      const std::vector<float>& inputs,
                      const std::vector<float>& expected_outputs_for_net) {
  double error_cost{};
  uint32_t layer_count = weights.size();
  std::vector<std::vector<float>> sums(layer_count);
  std::vector<std::vector<float>> outputs;
  std::vector<float> ret =
      forward_propgate_one_layer(weights[0], inputs, sums[0]);
  outputs.push_back(ret);
  for (uint32_t i{1}; i < layer_count; i++) {
    outputs.push_back(forward_propgate_one_layer(
        weights[i],
        outputs[static_cast<std::vector<
                    std::vector<float, std::allocator<float>>,
                    std::allocator<std::vector<float, std::allocator<float>>>>::
                                size_type>(i) -
                1],
        sums[i]));
  }
  uint32_t last_layer = layer_count - 1;
  std::vector<std::vector<float>> expected_outputs(layer_count - 1);
  expected_outputs[last_layer - 1] = calculate_one_layer(
      weights[last_layer], outputs[last_layer - 1], expected_outputs_for_net,
      sums[last_layer], outputs[last_layer], error_cost);

  for (uint32_t i{last_layer - 1}; 0 < i; i--) {
    expected_outputs[i - 1] =
        calculate_one_layer(weights[i], outputs[i - 1], expected_outputs[i],
                            sums[i], outputs[i], error_cost);
  }
  calculate_one_layer(weights[0], inputs, expected_outputs[0], sums[0],
                      outputs[0], error_cost);
  return error_cost;
}

int main() {
  std::vector<std::vector<float>> inputs;

  std::vector<std::vector<std::vector<float>>> weights = {
      {{1, 1, 1}, {1, 1, 1}}, {{1, 1, 1}, {1, 1, 1}}, {{1, 1, 1}, {1, 1, 1}}};
  std::vector<std::vector<float>> expected_outputs;
  for (uint32_t i{}; i < 1000; i++) {
    float a = (std::rand() % 50) / 5.0f;
    float b = (std::rand() % 50) / 5.0f;
    inputs.push_back({a, b});
    expected_outputs.push_back({(a + b) / 2, (a + b)});
  }

  uint32_t training_data_count = expected_outputs.size();
  double pervious_error = 100;
  std::vector<float> ei(training_data_count);
  while (1) {
    double error_cost{};
    for (uint32_t i{}; i < training_data_count; i++) {
      error_cost += back_propagate(weights, inputs[i], expected_outputs[i]);
    }
    error_cost /= training_data_count;
    std::cout << "error is :" << error_cost << " \n";
    if (abs(error_cost - pervious_error) < 0.00000001)
      break;
    pervious_error = error_cost;
  }
  // mjz_print(weights);
  mjz_print(pridict(weights, {10, 5}));
  return 0;
}
} // namespace my_failed_neural_net