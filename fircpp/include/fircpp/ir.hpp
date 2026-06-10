#pragma once
#include "fir/ir.h"
#include "fircpp/block.hpp"
#include "fircpp/func.hpp"
#include "fircpp/type.hpp"
#include "fircpp/value.hpp"
#include <array>
#include <memory>
#include <string>
#include <vector>

#undef alloca // fucking alloca.h, who decided it was a good idea to do make
              // this macro?

namespace fir {
class IR {
public:
  IR();
  IR(const IR &) = delete;
  IR &operator=(IR &) = delete;
  IR(IR &&) noexcept;
  IR &operator=(IR &&) noexcept;
  ~IR();

  void print(FILE *out);
  Func func(type::Type type, std::string name,
            std::array<type::Type, FR_FUNC_MAX_ARGS> args);
  Func extfunc(type::Type type, std::string name,
               std::array<type::Type, FR_FUNC_MAX_ARGS> args);
  Block block();
  void set_insert_point(Block block);
  void set_insert_point(Func func);
  Value arg(size_t index);

  Value alloca(type::Type type);
  void store(Value &dest, Value &src);
  Value load(Value &value, type::Type type);
  Value constant(type::Type type, long number);
  Value constant(type::Type type, double number);
  Value constant(const std::string &s);
  void ret(Value &value);
  Value add(Value &lhs, Value &rhs);
  Value sub(Value &lhs, Value &rhs);
  Value mul(Value &lhs, Value &rhs);
  Value div(Value &lhs, Value &rhs);
  Value eq(Value &lhs, Value &rhs);
  Value neq(Value &lhs, Value &rhs);
  Value lt(Value &lhs, Value &rhs);
  Value gt(Value &lhs, Value &rhs);
  Value lte(Value &lhs, Value &rhs);
  Value gte(Value &lhs, Value &rhs);
  Value bit_and(Value &lhs, Value &rhs);
  Value bit_or(Value &lhs, Value &rhs);
  Value bit_xor(Value &lhs, Value &rhs);
  Value bit_shl(Value &lhs, Value &rhs);
  Value bit_shr(Value &lhs, Value &rhs);
  Value select(Value &test, Value &true_value, Value &false_value);
  void br(Block block);
  void cond_br(Value &value, Block true_block, Block false_block);
  Value cast(Value &value, type::Type type);
  Value call(Func &func, std::array<Value *, FR_FUNC_MAX_ARGS> args);
  Value arrindex(Value &array, Value &index);

  std::shared_ptr<frIRType> create_new_type();
  const char *unowned_str(const std::string &from);

private:
  frIR ir;
  std::vector<std::shared_ptr<frIRType>> types;
};
} // namespace fir
