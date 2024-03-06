
#include "my_main.h"
namespace mjz_ard {}  // namespace mjz_ard
/*
 *for some unknown reason the auto complete doesn't work on my msvc for big
 *classes like the optional  . PLANS :
 *time is limited so  the minimal  mjz string should wait a bit  for
 *the revamp of the allocators
 *and some mess that should be cleaned and then for the vectors and the classes
 *dependent on dynamic allocators to update api and finally then the minimal mjz
 *string will be written
 * and the unique and sheared and weak ptr will be next

 */

struct public_key_t{
  std::vector<std::pair<std::vector<int>, int64_t>> public_key_vectors;
  uint64_t public_key_mod{};
};

struct   private_key_t {
  std::vector<int> private_key;
  uint64_t private_key_mod{};
};
  struct key_pair_t{
  private_key_t private_k;
  public_key_t public_k;
  };
struct massage_t {
    std::vector<int>vector_formula;
  int64_t massage_value_with_formula{};
    uint64_t public_key_mod{};
};

const constexpr static size_t Num_for_encryption_vector = 128;
const constexpr static size_t Num_for_encryption_vector_fourmula = 128;
_NODISCARD key_pair_t get_key_pair( ) {
  key_pair_t p;
    auto& private_key = p.private_k.private_key;
  auto& public_key_vectors = p.public_k.public_key_vectors;
  auto& public_key_mod = p.public_k.public_key_mod;
  private_key.resize(Num_for_encryption_vector);
  for (auto& pram : private_key) {
    pram = (std::rand())+1;
  }

  while (public_key_mod <= UINT_MAX)
    public_key_mod +=
        uint64_t(std::rand()) ^ uint64_t(uint64_t(std::rand()) << 32);
 p.private_k. private_key_mod = public_key_mod;
  private_key.resize(Num_for_encryption_vector);
  public_key_vectors.resize(Num_for_encryption_vector_fourmula);
  for (auto& [v, answer] : public_key_vectors) {
    v.reserve(Num_for_encryption_vector);
    v.resize(Num_for_encryption_vector);
    answer = 0;
    auto it_pvk_pram = private_key.begin();
    for (int& pram_mult : v) {
      pram_mult = std::rand() >> 8;
      answer += pram_mult * (*it_pvk_pram);
      ++it_pvk_pram;
    }
    answer += (static_cast<uint8_t>(std::rand()) % 7) - 3;
    answer %= public_key_mod;
  }
  return p;
}
_NODISCARD
massage_t make_bool_massage(const public_key_t& public_key,
                           bool massage_Boolean) {
    massage_t m;
 auto& public_key_mod= public_key.public_key_mod;
    auto& public_key_vectors= public_key.public_key_vectors;
 size_t num_of_prams = public_key_vectors[0].first.size();
    m.vector_formula.resize(num_of_prams);
    m.public_key_mod = public_key_mod;
    size_t number_of_formula{public_key_vectors.size()};
   auto public_key_vectors_it = public_key_vectors.begin();
    std::vector<int> furmula_multiplier_vector;
   furmula_multiplier_vector.resize(number_of_formula);
    for (int& multiplier : furmula_multiplier_vector) {
    multiplier= std::rand()>>8;
 }

    auto mut_it = furmula_multiplier_vector.begin();
 for (size_t i{}; i < number_of_formula;
      i++, ++mut_it, ++public_key_vectors_it) {
    auto& formula_vector = public_key_vectors_it->first;
    auto formula_vector_result= public_key_vectors_it->second;
    auto pram_mult_it = formula_vector.begin();
    for (int j{}; j < num_of_prams; j++, ++pram_mult_it) {
      m.vector_formula[j] += *pram_mult_it * (*mut_it);
    }
    m.massage_value_with_formula += formula_vector_result * (*mut_it);
    }
 m.massage_value_with_formula += (static_cast<uint8_t>(std::rand()) % 7) - 3;

    m.massage_value_with_formula += (massage_Boolean * public_key_mod) >> 1;
 return m;
}

struct get_bool_massage_t {
 bool was_succsessful = true;
 bool result{};
};
_NODISCARD
get_bool_massage_t get_bool_massage(const private_key_t& private_key,
                         const massage_t& massage) {

 if (massage.public_key_mod != private_key.private_key_mod ||
     private_key.private_key.size() != massage.vector_formula.size())
    return {.was_succsessful = 0};
 auto key_mod= private_key.private_key_mod;
 auto& private_key_v = private_key.private_key;
 auto private_key_v_it = private_key_v.begin();
 auto massage_it = massage.vector_formula.begin();
 auto massage_value_with_formula = massage.massage_value_with_formula;
 auto number_of_prams = private_key.private_key.size();
 int64_t expected_value_with_formula{};
 for (int pram_i{}; pram_i < number_of_prams;

      pram_i++, ++massage_it, ++private_key_v_it) {
    expected_value_with_formula +=
        int64_t(*massage_it) * int64_t(*private_key_v_it);
 }
 auto private_key_mod= private_key.private_key_mod;
 expected_value_with_formula %= private_key_mod;
 massage_value_with_formula -= expected_value_with_formula;
 massage_value_with_formula = abs(massage_value_with_formula);
 return {.result = ((private_key_mod >> 4) < massage_value_with_formula)};
}

std::vector<massage_t> make_str_massage(const public_key_t& public_key,
                           const char* massage_str,size_t len) {
 std::vector<massage_t> massage_vector;
 if (!len || !massage_str) return massage_vector;
 massage_vector.reserve(len * 8);
 size_t i{};
 for (; i < len; i++) {
    for (int j{}; j < 8; j++)
    massage_vector.emplace_back(
          make_bool_massage(public_key, (massage_str[i] & (MJZ_logic_BIT(j)))
          ));
 }
 return massage_vector;
}

mjzt::succsess_t get_str_massage(const private_key_t& private_key,
                                 const std::vector<massage_t>& massages,
                                 char* massage_str_buffer, size_t len) {
 if (massages.size() % 8 || (massages.size()) != (len*8)) {
    return false;
 }
 memset(massage_str_buffer, 0, len);
 size_t i{};
 auto massages_it = massages.begin();
 for (; i < len; i++) {
    for (int j{}; j < 8; j++, ++massages_it) {
    auto result_v = get_bool_massage(private_key, *massages_it);
    if (!result_v.was_succsessful) {
      return false;
    }
    auto result = MJZ_logic_bit_to_8_bits(result_v.result);
    massage_str_buffer[i] |= result & MJZ_logic_BIT(j);
    }
 }
 return true;
}

int my_main::main(int argc, const char* const* const argv) {
 USE_MJZ_NS();
  key_pair_t pair = get_key_pair();
 std::string_view v = " 78967068956091815307-968319157-35195391-7631234356475890687864758693018571586397-0697-013968397036";
  auto m = make_str_massage(pair.public_k, v.data(), v.size());
 std::string str;
  str.resize( m.size()/8);
 bool result= get_str_massage(pair.private_k, m,str.data(),str.length());


  println(result ? "true" : "false", " : ", str==v ? "true" : "false" , " : ",
         str);


  return 0;
}
