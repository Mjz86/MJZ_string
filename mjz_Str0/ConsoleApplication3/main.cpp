
#include "my_main.h"

namespace mjz_ard {};  // namespace mjz_ard
namespace test {

using Bits_t = std::bitset<256>;
enum Attributes : uint8_t /*0 -> 256*/ {
  no_Entity = 0,
  alive,
  visible,
  killable,
  movable,
  colidable,
  debug,
  mob,
  player,
  render
};
struct Entity_attribute : public Bits_t {
  Entity_attribute() : Bits_t() {}
  Entity_attribute(const Entity_attribute& e) : Bits_t(e) {}
  Entity_attribute(Entity_attribute&& e) noexcept : Bits_t(std::move(e)) {}

  Entity_attribute& operator=(const Entity_attribute& e) {
    Bits_t::operator=(e);
    return *this;
  }
  Entity_attribute& operator=(Entity_attribute&& e) {
    Bits_t::operator=(std::move(e));
    return *this;
  }
};
struct UUID {
  UUID() {}
  UUID(size_t nameID, size_t randID) : name_ID(nameID), rand_ID(randID) {}
  UUID(size_t nameID)
      : name_ID(nameID),
        rand_ID(std::hash<size_t>()(
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch())
                .count())) {}
  size_t name_ID{};
  size_t rand_ID{};
  inline constexpr friend bool operator==(const UUID& a, const UUID& b) {
    return is_same_type(a, b) && a.rand_ID == b.rand_ID;
  }

  inline constexpr friend bool operator!=(const UUID& a, const UUID& b) {
    return !(a == b);
  }

  inline constexpr friend bool operator<(const UUID& a, const UUID& b) {
    if (is_same_type(a, b)) return a.rand_ID < b.rand_ID;
    return a.name_ID < b.name_ID;
  }

  inline constexpr friend bool operator<=(const UUID& a, const UUID& b) {
    return (a == b) || (a < b);
  }

  inline constexpr friend bool operator>(const UUID& a, const UUID& b) {
    return !(a <= b);
  }

  inline constexpr friend bool operator>=(const UUID& a, const UUID& b) {
    return !(a < b);
  }

  inline constexpr static bool is_same_type(const UUID& a, const UUID& b) {
    return (a.name_ID == b.name_ID);
  }
  inline constexpr bool is_same_type(const UUID& b) const {
    return is_same_type(*this, b);
  }
};
class Entity {
 public:
  /*
   UUID m_uuid;
  mjzt::optional<mjzt::mjz_Str> m_name;
 mjzt::mjz_Vectorf3 m_position;
 float m_health{-1};
Entity_attribute m_attributes{};
 */
 public:
  virtual mjzt::optional_ref<const mjzt::mjz_Str&> get_name() const = 0;
  virtual Entity& set_name(const mjzt::mjz_str_view&) = 0;
  virtual const UUID& get_UUID() const = 0;
  virtual Entity& set_UUID(const UUID&) = 0;
  virtual float get_health() const = 0;
  virtual Entity& set_health(float) = 0;
  virtual const mjzt::mjz_Vectorf3& get_position() const = 0;
  virtual Entity& set_position(const mjzt::mjz_Vectorf3&) = 0;
  virtual const Entity_attribute& get_attribute() const = 0;
  virtual Entity& set_attribute(const Entity_attribute&) = 0;
  virtual Entity& set_attribute_at(Attributes at, bool b = true) {
    Entity_attribute new_at = get_attribute();
    new_at[at] = b;
    return set_attribute(new_at);
  };
  virtual bool get_attribute_at(Attributes at) const {
    return get_attribute()[at];
  };
  Entity& reset_attribute_at(Attributes at) {
    return set_attribute_at(at, false);
  };
  Entity& clear_attribute_at(Attributes at) {
    return set_attribute_at(at, false);
  };

  Entity() {}
  virtual ~Entity() {}

  bool operator==(const Entity& other) const {
    return get_UUID() == other.get_UUID();
  };

  friend std::ostream& operator<<(std::ostream& COUT, const Entity& e) {
    USE_MJZ_NS();
    const auto& op_name = e.get_name();
    optional<mjz_str> str;
    if (e.get_attribute()[Attributes::debug]) {
      str.emplace("with_address address:") += &e;
    }
    if (op_name) {
      println("Entity ", *str.value_or_emplace(""), ":{.name=\"", op_name.get(),
              "\",.health=", e.get_health(), ",.ID=0x",
              (const void* const)e.get_UUID().name_ID,
              (const void* const)e.get_UUID().rand_ID,
              "} at position:", e.get_position());
      return COUT;
    }
    println("Entity", *str.value_or_emplace(""), " :{.health=", e.get_health(),
            '}', "at position:", e.get_position());

    return COUT;
  }
};

class Player : public Entity {
  constexpr static const char Entity_name[] = "Player";
  const static inline size_t Player_ID =
      std::hash<mjzt::mstrview>()(Entity_name);
  mjzt::optional<mjzt::mjz_Str> m_name;
  mjzt::mjz_Vectorf3 m_position;
  float m_health{-1};
  Entity_attribute m_attributes{};
  UUID m_uuid{Player_ID};

 public:
  mjzt::optional_ref<const mjzt::mjz_Str&> get_name() const override {
    return m_name.uop();
  }
  Entity& set_name(const mjzt::mjz_str_view& name) override {
    m_name.emplace(name);
    return *this;
  }
  const UUID& get_UUID() const override { return m_uuid; }
  Entity& set_UUID(const UUID& uuid) override {
    m_uuid = uuid;
    return *this;
  }
  float get_health() const override { return m_health; }
  Entity& set_health(float hp) override {
    m_health = hp;
    return *this;
  }
  const mjzt::mjz_Vectorf3& get_position() const override { return m_position; }
  Entity& set_position(const mjzt::mjz_Vectorf3& p) override {
    m_position = p;
    return *this;
  }
  const Entity_attribute& get_attribute() const override {
    return m_attributes;
  }
  Entity& set_attribute(const Entity_attribute& at) override {
    m_attributes = at;
    return *this;
  }

  Entity& set_attribute_at(Attributes at, bool b = true) override {
    m_attributes[at] = b;
    return *this;
  }
};
}  // namespace test

int my_main::main(int argc, const char* const* const argv) {
  USE_MJZ_NS();
  if (auto o = optional<operation_reporter>("name")) {
    o()++;
  }
  return 0;
}

