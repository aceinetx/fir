#include "fircpp/type.hpp"
#include "fircpp/ir.hpp"

fir::type::Type::Type(frIRType *type) : type(type) {}
fir::type::Type::Type(std::nullptr_t) : type(nullptr) {}
fir::type::Type::Type() : type(nullptr) {}

fir::type::Type fir::type::Type::builtin(IR &ir, std::string repr) {
  auto type = ir.create_new_type();
  type->unit = FR_TYPE_BUILTIN;
  type->v.builtin.repr = ir.unowned_str(repr);
  return {type.get()};
}

fir::type::Type fir::type::Type::ptr(IR &ir, Type pointee) {
  auto type = ir.create_new_type();
  type->unit = FR_TYPE_PTR;
  type->v.ptr.pointee = pointee;
  return {type.get()};
}

fir::type::Type fir::type::Type::array(IR &ir, Type element_type, size_t size) {
  auto type = ir.create_new_type();
  type->unit = FR_TYPE_ARR;
  type->v.array.type = element_type;
  type->v.array.size = size;
  return {type.get()};
}

fir::type::Type
fir::type::Type::funcptr(IR &ir, Type ret_type,
                         std::array<Type, FR_FUNC_MAX_ARGS> arg_types) {
  auto type = ir.create_new_type();
  type->unit = FR_TYPE_ARR;
  type->v.funcptr.ret_type = ret_type;
  for (int i = 0; i < FR_FUNC_MAX_ARGS; i++) {
    type->v.funcptr.arg_types[i] = arg_types[i];
  }
  return {type.get()};
}

fir::type::Type::operator frIRType *() { return type; }
fir::type::Type::operator const frIRType *() { return type; }
