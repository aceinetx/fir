#pragma once
#include "fir/ir_type.h"
#include <string>

namespace fir {
class IR;
namespace type {
struct Type {
  Type(frIRType *type);
  Type(std::nullptr_t);
  Type();

  static Type builtin(IR &ir, std::string repr);
  static Type ptr(IR &ir, Type pointee);
  static Type array(IR &ir, Type element_type, size_t size);
  static Type funcptr(IR &ir, Type ret_type,
                      std::array<Type, FR_FUNC_MAX_ARGS> arg_types);

  operator frIRType *();
  operator const frIRType *();

private:
  frIRType *type;
};

const auto i8 = Type(&frIRType_i8);
const auto i16 = Type(&frIRType_i16);
const auto i32 = Type(&frIRType_i32);
const auto i64 = Type(&frIRType_i64);
const auto u0 = Type(&frIRType_u0);
const auto u1 = Type(&frIRType_u1);
const auto u8 = Type(&frIRType_u8);
const auto u16 = Type(&frIRType_u16);
const auto u32 = Type(&frIRType_u32);
const auto u64 = Type(&frIRType_u64);
const auto f32 = Type(&frIRType_f32);
const auto f64 = Type(&frIRType_f64);
const auto str = Type(&frIRType_str);
} // namespace type
} // namespace fir
