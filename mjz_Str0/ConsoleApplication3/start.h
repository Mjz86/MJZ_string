#pragma once

#include "mjzString.hpp"
class main_class {
 std::unique_ptr<main_class> run(int argc, const char* const* const argv);

 protected:
 inline virtual int main(int argc, const char* const* const argv) { return 0; }
 inline virtual bool catch_exceptions() { return false; }

 public:
 inline int setup(int argc, const char* const* const argv) {
 int return_val{-1};
 std::unique_ptr<main_class> ptr = run(argc, argv);
 if (ptr->catch_exceptions()) {
 try {
 try {
 return_val = ptr->main(argc, argv);
 } catch (std::exception ex) {
 std::cout << "\n\n\n EXEPTION : " << ex.what();
 } catch (mjz_ard::StringSumHelper ex) {
 std::cout << "\n\n\n EXEPTION : " << ex;
 } catch (mjz_ard::basic_mjz_Str_view ex) {
 std::cout << "\n\n\n EXEPTION : " << ex;
 }
 } catch (...) {
 std::cout << "\n\n\n EXEPTION happened :( ";
 }
 } else {
 return_val = ptr->main(argc, argv);
 }
 std::cout << std::endl << std::endl;
 return return_val;
 }
 inline virtual ~main_class() = default;
 main_class() = default;
 main_class(main_class&&) = delete;
 main_class(const main_class&) = delete;
 main_class operator=(main_class&&) = delete;
 main_class operator=(const main_class&) = delete;
};
