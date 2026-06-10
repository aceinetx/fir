#include "fircpp/ir.hpp"
#include <cstring>

fir::IR::IR() : ir(frIR_new()) {}

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
