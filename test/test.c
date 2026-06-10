#include "fir/ir.h"
#include <stdlib.h>

int main() {
  frIR ir = frIR_new();

  char *s = malloc(2);
  s[0] = 'a';
  s[1] = 0;

  frIR_func(&ir, &frIRType_i32, s, FR_FUNC_TYPE_ARRAY(NULL));

  free(s);

  frIR_print(&ir, stdout);

  frIR_free(&ir);
}
