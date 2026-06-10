#include "fir/ir_inst.h"

void frIRInst_print(frIRInst *inst, FILE *out) {
  switch (inst->type) {
  case FR_ALLOCA: {
    /* Isn't handled here anymore */
    /* See frIRBlock_print_decls and frIRFunc_print */
  } break;
  case FR_RET: {
    fprintf(out, "return %s;\n", inst->v.ret.name ? inst->v.ret.name : "");
  } break;
  case FR_CONST_INT: {
    fprintf(out, "%s =%ld;\n", inst->v.const_int.name,
            inst->v.const_int.number);
  } break;
  case FR_CONST_FLOAT: {
    fprintf(out, "%s =%lf;\n", inst->v.const_float.name,
            inst->v.const_float.number);
  } break;
  case FR_CONST_STR: {
    fprintf(out, "{uint8_t tmp[] = {");
    const char *c = inst->v.const_str.str;
    while (*c) {
      fprintf(out, "%d, ", *c);
      c++;
    }
    fprintf(out, "0}; %s =tmp; }\n", inst->v.const_str.name);
  } break;
  case FR_STORE: {
    fprintf(out, "*%s=%s;\n", inst->v.store.name_dest, inst->v.store.name_src);
  } break;
  case FR_LOAD: {
    fprintf(out, "%s =*%s;\n", inst->v.load.name, inst->v.load.source);
  } break;
  case FR_BINOP: {
    char *op = NULL;

    switch (inst->v.binop.op) {
    case FR_BINOP_ADD:
      op = "+";
      break;
    case FR_BINOP_SUB:
      op = "-";
      break;
    case FR_BINOP_MUL:
      op = "*";
      break;
    case FR_BINOP_DIV:
      op = "/";
      break;
    case FR_BINOP_EQUAL:
      op = "==";
      break;
    case FR_BINOP_NEQUAL:
      op = "!=";
      break;
    case FR_BINOP_LT:
      op = "<";
      break;
    case FR_BINOP_GT:
      op = ">";
      break;
    case FR_BINOP_LTE:
      op = "<=";
      break;
    case FR_BINOP_GTE:
      op = ">=";
      break;
    case FR_BINOP_AND:
      op = "&";
      break;
    case FR_BINOP_OR:
      op = "|";
      break;
    case FR_BINOP_XOR:
      op = "^";
      break;
    case FR_BINOP_SHL:
      op = "<<";
      break;
    case FR_BINOP_SHR:
      op = ">>";
      break;
    }

    fprintf(out, "%s =%s%s%s;\n", inst->v.binop.name, inst->v.binop.name_lhs,
            op, inst->v.binop.name_rhs);
  } break;
  case FR_SELECT: {
    frIRType_print(inst->v.select.type, out, inst->v.select.name);
    fprintf(out, " =%s?%s:%s;\n", inst->v.select.name_test,
            inst->v.select.name_true, inst->v.select.name_false);
  } break;
  case FR_BR: {
    fprintf(out, "goto %s;\n", inst->v.br.block_name);
  } break;
  case FR_COND_BR: {
    fprintf(out, "if(%s) goto %s; goto %s;\n", inst->v.cond_br.name_value,
            inst->v.cond_br.true_block, inst->v.cond_br.false_block);
  } break;
  case FR_CAST: {
    fprintf(out, "%s =(", inst->v.cast.name);
    frIRType_print(inst->v.cast.type, out, NULL);
    fprintf(out, ")%s;\n", inst->v.cast.name_cast);
  } break;
  case FR_CALL: {
    fprintf(out, "%s =%s(", inst->v.call.name, inst->v.call.func);
    for (int i = 0; i < FR_FUNC_MAX_ARGS; i++) {
      const char *arg = inst->v.call.args[i];
      if (!arg)
        break;
      if (i > 0)
        fputs(",", out);
      fprintf(out, "%s", arg);
    }
    fputs(");\n", out);
  } break;
  case FR_ARRINDEX: {
    fprintf(out, "%s = &%s[%s];\n", inst->v.arrindex.name,
            inst->v.arrindex.array, inst->v.arrindex.index);
  } break;
  default:
    fprintf(out, "/* unknown instruction %d */", inst->type);
  }
}
