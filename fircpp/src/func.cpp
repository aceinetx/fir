#include "fircpp/func.hpp"

fir::Func::Func(frIRFunc *func) : func(func) {}

fir::Func::operator frIRFunc *() { return func; }

void fir::Func::header_print(FILE *out) { frIRFunc_header_print(func, out); }

void fir::Func::print(FILE *out) { frIRFunc_print(func, out); }
