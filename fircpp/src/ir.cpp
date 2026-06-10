#include "fircpp/ir.hpp"
#include <cstring>

fir::IR::IR() : ir(frIR_new()), types({}) {}

fir::IR::IR(IR &&other) noexcept : ir(other.ir) {
  std::memset(&other.ir, 0, sizeof other.ir);
}

fir::IR &fir::IR::operator=(IR &&other) noexcept {
  if (this != &other) {
    ir = other.ir;
    std::memset(&other.ir, 0, sizeof other.ir);
  }
  return *this;
}

fir::IR::~IR() { frIR_free(&ir); }

void fir::IR::print(FILE *out) { frIR_print(&ir, out); }

fir::Func fir::IR::func(fir::type::Type type, std::string name,
                        std::array<type::Type, FR_FUNC_MAX_ARGS> args) {
  std::array<frIRType *, FR_FUNC_MAX_ARGS> args_c{nullptr};
  int i = 0;
  for (i = 0; i < FR_FUNC_MAX_ARGS; i++)
    args_c[i] = args[i];
  return frIR_func(&ir, type, name.c_str(), args_c.data());
}

fir::Func fir::IR::extfunc(type::Type type, std::string name,
                           std::array<type::Type, FR_FUNC_MAX_ARGS> args) {
  std::array<frIRType *, FR_FUNC_MAX_ARGS> args_c{nullptr};
  int i = 0;
  for (i = 0; i < FR_FUNC_MAX_ARGS; i++)
    args_c[i] = args[i];
  return frIR_extfunc(&ir, type, name.c_str(), args_c.data());
}

fir::Block fir::IR::block() { return frIR_block(&ir); }

void fir::IR::set_insert_point(Block block) {
  frIR_set_insert_block(&ir, block);
}

void fir::IR::set_insert_point(Func func) { frIR_set_insert_func(&ir, func); }

fir::Value fir::IR::arg(size_t index) { return frIR_arg(&ir, index); }

fir::Value fir::IR::alloca(fir::type::Type type) {
  return frIR_alloca(&ir, type);
}

void fir::IR::store(Value &dest, Value &src) {
  return frIR_store(&ir, &dest, &src);
}

fir::Value fir::IR::load(Value &value, type::Type type) {
  return frIR_load(&ir, &value, type);
}

fir::Value fir::IR::constant(type::Type type, long number) {
  return frIR_constint(&ir, type, number);
}

fir::Value fir::IR::constant(type::Type type, double number) {
  return frIR_constfloat(&ir, type, number);
}

fir::Value fir::IR::constant(const std::string &s) {
  return frIR_conststr(&ir, s.c_str());
}

void fir::IR::ret(Value &value) { return frIR_ret(&ir, &value); }

fir::Value fir::IR::add(Value &lhs, Value &rhs) {
  return frIR_add(&ir, &lhs, &rhs);
}

fir::Value fir::IR::sub(Value &lhs, Value &rhs) {
  return frIR_sub(&ir, &lhs, &rhs);
}

fir::Value fir::IR::mul(Value &lhs, Value &rhs) {
  return frIR_mul(&ir, &lhs, &rhs);
}

fir::Value fir::IR::div(Value &lhs, Value &rhs) {
  return frIR_div(&ir, &lhs, &rhs);
}

fir::Value fir::IR::eq(Value &lhs, Value &rhs) {
  return frIR_eq(&ir, &lhs, &rhs);
}

fir::Value fir::IR::neq(Value &lhs, Value &rhs) {
  return frIR_neq(&ir, &lhs, &rhs);
}

fir::Value fir::IR::lt(Value &lhs, Value &rhs) {
  return frIR_lt(&ir, &lhs, &rhs);
}

fir::Value fir::IR::gt(Value &lhs, Value &rhs) {
  return frIR_gt(&ir, &lhs, &rhs);
}

fir::Value fir::IR::lte(Value &lhs, Value &rhs) {
  return frIR_lte(&ir, &lhs, &rhs);
}

fir::Value fir::IR::gte(Value &lhs, Value &rhs) {
  return frIR_gte(&ir, &lhs, &rhs);
}

fir::Value fir::IR::bit_and(Value &lhs, Value &rhs) {
  return frIR_bitand(&ir, &lhs, &rhs);
}

fir::Value fir::IR::bit_or(Value &lhs, Value &rhs) {
  return frIR_bitor(&ir, &lhs, &rhs);
}

fir::Value fir::IR::bit_xor(Value &lhs, Value &rhs) {
  return frIR_bitxor(&ir, &lhs, &rhs);
}

fir::Value fir::IR::bit_shl(Value &lhs, Value &rhs) {
  return frIR_bitshl(&ir, &lhs, &rhs);
}

fir::Value fir::IR::bit_shr(Value &lhs, Value &rhs) {
  return frIR_bitshr(&ir, &lhs, &rhs);
}

fir::Value fir::IR::select(Value &test, Value &true_value, Value &false_value) {
  return frIR_select(&ir, &test, &true_value, &false_value);
}

void fir::IR::br(Block block) { return frIR_br(&ir, block); }

void fir::IR::cond_br(Value &value, Block true_block, Block false_block) {
  return frIR_cond_br(&ir, &value, true_block, false_block);
}

fir::Value fir::IR::cast(Value &value, type::Type type) {
  return frIR_cast(&ir, &value, type);
}

fir::Value fir::IR::call(Func &func,
                         std::array<Value *, FR_FUNC_MAX_ARGS> args) {
  return frIR_call(&ir, func, args.data());
}

void fir::IR::arrset(frIRValue &array, frIRValue &index, frIRValue &value) {
  frIR_arrset(&ir, &array, &index, &value);
}

fir::Value fir::IR::arrget(Value &array, Value &index) {
  return frIR_arrget(&ir, &array, &index);
}

std::shared_ptr<frIRType> fir::IR::create_new_type() {
  types.push_back(std::make_shared<frIRType>());
  return types.back();
}

const char *fir::IR::unowned_str(const std::string &from) {
  return frIR_strdup(&ir, from.c_str());
}
