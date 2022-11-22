#ifndef __BINDINGS_WABT_H
#define __BINDINGS_WABT_H
#ifdef __cplusplus
extern "C"
{
  #endif
  
  #include <stdint.h>
  #include <stdbool.h>
  
  typedef struct {
    char *ptr;
    size_t len;
  } wabt_string_t;
  
  void wabt_string_set(wabt_string_t *ret, const char *s);
  void wabt_string_dup(wabt_string_t *ret, const char *s);
  void wabt_string_free(wabt_string_t *ret);
  typedef uint16_t wabt_wasm_feature_t;
  #define WABT_WASM_FEATURE_EXCEPTIONS (1 << 0)
  #define WABT_WASM_FEATURE_MUTABLE_GLOBALS (1 << 1)
  #define WABT_WASM_FEATURE_SAT_FLOAT_TO_INT (1 << 2)
  #define WABT_WASM_FEATURE_SIGN_EXTENSION (1 << 3)
  #define WABT_WASM_FEATURE_SIMD (1 << 4)
  #define WABT_WASM_FEATURE_THREADS (1 << 5)
  #define WABT_WASM_FEATURE_MULTI_VALUE (1 << 6)
  #define WABT_WASM_FEATURE_TAIL_CALL (1 << 7)
  #define WABT_WASM_FEATURE_BULK_MEMORY (1 << 8)
  #define WABT_WASM_FEATURE_REFERENCE_TYPES (1 << 9)
  #define WABT_WASM_FEATURE_ANNOTATIONS (1 << 10)
  #define WABT_WASM_FEATURE_GC (1 << 11)
  typedef struct {
    uint8_t *ptr;
    size_t len;
  } wabt_list_u8_t;
  void wabt_list_u8_free(wabt_list_u8_t *ptr);
  typedef struct {
    bool is_err;
    union {
      wabt_list_u8_t ok;
      wabt_string_t err;
    } val;
  } wabt_expected_list_u8_string_t;
  void wabt_expected_list_u8_string_free(wabt_expected_list_u8_string_t *ptr);
  typedef struct {
    bool is_err;
    union {
      wabt_string_t ok;
      wabt_string_t err;
    } val;
  } wabt_expected_string_string_t;
  void wabt_expected_string_string_free(wabt_expected_string_string_t *ptr);
  void wabt_wat2wasm(wabt_string_t *wat, wabt_wasm_feature_t features, wabt_expected_list_u8_string_t *ret0);
  void wabt_wasm2wat(wabt_list_u8_t *wasm, wabt_wasm_feature_t features, wabt_expected_string_string_t *ret0);
  #ifdef __cplusplus
}
#endif
#endif
