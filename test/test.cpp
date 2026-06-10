#include "fircpp/ir.hpp"
#include "fircpp/type.hpp"

int main() {
  fir::IR ir;
  fir::Func add =
      ir.func(fir::type::i32, "add", {fir::type::i32, fir::type::i32});
  fir::Value a = ir.arg(0);
  fir::Value b = ir.arg(1);
  fir::Value ret = ir.add(a, b);
  ir.ret(ret);

  fir::Func main = ir.func(fir::type::i32, "main", {fir::type::i32});
  fir::Value argc = ir.arg(0);
  ret = ir.call(add, {&argc, nullptr});
  ir.ret(ret);

  ir.print(stdout);
}
