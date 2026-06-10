#include "fir/ir.h"
#include <stdlib.h>

int main() {
  frIR ir = frIR_new();

  frIR_func(&ir, &frIRType_i32, "main", FR_FUNC_TYPE_ARRAY(NULL));
  frIRType array_type = (frIRType){
      .unit = FR_TYPE_ARR, .v.array = {.size = 8, .type = &frIRType_i32}};
  frIRValue array = frIR_alloca(&ir, &array_type);
  frIRValue zero = frIR_constint(&ir, &frIRType_i32, 0);

  frIRValue indexed = frIR_arrindex(&ir, &array, &zero);
  frIR_store(&ir, &indexed, &zero);
  frIRValue ret = frIR_load(&ir, &indexed, &frIRType_i32);
  frIR_ret(&ir, &ret);

  frIR_print(&ir, stdout);

  frIR_free(&ir);
}
