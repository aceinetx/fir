#ifndef FEATHER_CONFIG_H
#define FEATHER_CONFIG_H

/* extern C config */
#if __cplusplus
#define FR_HEADER_BEGIN extern "C" {
#define FR_HEADER_END }
#else
#define FR_HEADER_BEGIN
#define FR_HEADER_END
#endif

FR_HEADER_BEGIN

/* frontend config */
#define FR_IDENT_SIZE 8

/* backend config */
#define FR_FUNC_MAX_ARGS 8
#define FR_IR_ARENA_SIZE 0x2000

extern const char *FR_FUNC_ARG_NAMES[FR_FUNC_MAX_ARGS];

/* memory config */
#define FR_MEMORY_ALIGN 1

#if FR_MEMORY_ALIGN
#define FR_ALIGN(n, align) (((n) + (align) - 1) & ~((align) - 1))
#else
#define FR_ALIGN(n, align) (n)
#endif

/* convenience macros */
#define FR_FUNC_TYPE_ARRAY(...) ((frIRType *[FR_FUNC_MAX_ARGS]){__VA_ARGS__})
#define FR_FUNC_ARG_ARRAY(...) ((frIRValue *[FR_FUNC_MAX_ARGS]){__VA_ARGS__})

FR_HEADER_END

#endif
