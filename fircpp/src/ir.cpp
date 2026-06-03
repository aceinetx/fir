#include "fircpp/ir.hpp"
#include <cstring>

fir::IR::IR() : ir(frIR_new()) {}

fir::IR::IR(IR &&other) {
  ir = other.ir;
  std::memset(&other.ir, 0, sizeof other.ir);
}

fir::IR &fir::IR::operator=(IR &&other) {
  if (this != &other) {
    ir = other.ir;
    std::memset(&other.ir, 0, sizeof other.ir);
  }
  return *this;
}

fir::IR::~IR() { frIR_free(&ir); }
