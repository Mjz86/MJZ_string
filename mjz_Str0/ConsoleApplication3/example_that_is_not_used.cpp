
#include "my_main.h"
namespace test{
  class Entity {
  mjzt::operation_reporter o;
    Entity() {}
    virtual~Entity() {}
    Entity(Entity &&obj) {}
    Entity(Entity &obj) {}
    Entity(const Entity &obj) {}
    Entity(const Entity &&obj) {}
    Entity &operator=(Entity &&obj) {}
    Entity &operator=(Entity &obj) {}
    Entity &operator=(const Entity &obj) {}
    Entity &operator=(const Entity &&obj) {}


    inline static void *operator new(size_t, void *where) { return where; }
    inline static void *operator new[](size_t, void *where) { return where; }
    inline static void operator delete(void *, void *) {}
    inline static void operator delete(void *, size_t, void *) {}
    inline static void operator delete[](void *, void *) {}
    inline static void operator delete[](void *, size_t, void *) {}

      
    static inline mjzt::allocator_warpper<char> &allocator() {
      static mjzt::allocator_warpper<char> allocator_obj;
      return allocator_obj;
    }
    inline static [[nodiscard]] void *operator new(size_t n) {
      return allocator ().allocate_obj_array(n);
    }
    inline static [[nodiscard]] void *operator new[](size_t n) {
      return allocator().allocate_obj_array(n);
    }
  
    inline static void operator delete(void *p) {
          allocator().deallocate_obj_array((char*)p);
    }
    inline static void operator delete[](void *p) {
          allocator().deallocate_obj_array((char *)p);
    }
    inline static void operator delete(void *p, size_t) {
          allocator().deallocate_obj_array((char *)p);
    }
    inline static void operator delete[](void *p, size_t) {
          allocator().deallocate_obj_array((char *)p);
    }
    friend mjz::mjz_obj_manager_template_t<Entity>;

   public:
  virtual mjzt::optional<mjzt::mjz_Str> get( ) {
      using namespace mjz::short_string_convestion_operators;
          mjzt::mjz_Str str =  o.UUID();
      return {" no name with UUID"_s + str};
    }

  };
}


int my_main::main(int argc, const char *const *const argv) {
  using namespace mjz;
  using namespace mjzt;
  mjz_heap_obj_warper<test::Entity> e;
  (+e)->get()([](mjz_Str &&o) {
      println(o);
              });
  return 0;
}
