#include <stdlib.h>
#include "./wabt.h"

__attribute__((weak, export_name("canonical_abi_realloc")))
void *canonical_abi_realloc(
void *ptr,
size_t orig_size,
size_t org_align,
size_t new_size
) {
  void *ret = realloc(ptr, new_size);
  if (!ret)
  abort();
  return ret;
}

__attribute__((weak, export_name("canonical_abi_free")))
void canonical_abi_free(
void *ptr,
size_t size,
size_t align
) {
  free(ptr);
}
#include <string.h>

void wabt_string_set(wabt_string_t *ret, const char *s) {
  ret->ptr = (char*) s;
  ret->len = strlen(s);
}

void wabt_string_dup(wabt_string_t *ret, const char *s) {
  ret->len = strlen(s);
  ret->ptr = canonical_abi_realloc(NULL, 0, 1, ret->len);
  memcpy(ret->ptr, s, ret->len);
}

void wabt_string_free(wabt_string_t *ret) {
  canonical_abi_free(ret->ptr, ret->len, 1);
  ret->ptr = NULL;
  ret->len = 0;
}
void wabt_list_u8_free(wabt_list_u8_t *ptr) {
  canonical_abi_free(ptr->ptr, ptr->len * 1, 1);
}
void wabt_expected_list_u8_string_free(wabt_expected_list_u8_string_t *ptr) {
  if (!ptr->is_err) {
    wabt_list_u8_free(&ptr->val.ok);
  } else {
    wabt_string_free(&ptr->val.err);
  }
}
void wabt_expected_string_string_free(wabt_expected_string_string_t *ptr) {
  if (!ptr->is_err) {
    wabt_string_free(&ptr->val.ok);
  } else {
    wabt_string_free(&ptr->val.err);
  }
}

__attribute__((aligned(4)))
static uint8_t RET_AREA[12];
__attribute__((export_name("wat2wasm")))
int32_t __wasm_export_wabt_wat2wasm(int32_t arg, int32_t arg0, int32_t arg1) {
  wabt_string_t arg2 = (wabt_string_t) { (char*)(arg), (size_t)(arg0) };
  wabt_expected_list_u8_string_t ret;
  wabt_wat2wasm(&arg2, arg1, &ret);
  int32_t ptr = (int32_t) &RET_AREA;
  
  if ((ret).is_err) {
    const wabt_string_t *payload3 = &(ret).val.err;
    *((int8_t*)(ptr + 0)) = 1;
    *((int32_t*)(ptr + 8)) = (int32_t) (*payload3).len;
    *((int32_t*)(ptr + 4)) = (int32_t) (*payload3).ptr;
    
  } else {
    const wabt_list_u8_t *payload = &(ret).val.ok;
    *((int8_t*)(ptr + 0)) = 0;
    *((int32_t*)(ptr + 8)) = (int32_t) (*payload).len;
    *((int32_t*)(ptr + 4)) = (int32_t) (*payload).ptr;
    
  }
  return ptr;
}
__attribute__((export_name("wasm2wat")))
int32_t __wasm_export_wabt_wasm2wat(int32_t arg, int32_t arg0, int32_t arg1) {
  wabt_list_u8_t arg2 = (wabt_list_u8_t) { (uint8_t*)(arg), (size_t)(arg0) };
  wabt_expected_string_string_t ret;
  wabt_wasm2wat(&arg2, arg1, &ret);
  int32_t ptr = (int32_t) &RET_AREA;
  
  if ((ret).is_err) {
    const wabt_string_t *payload3 = &(ret).val.err;
    *((int8_t*)(ptr + 0)) = 1;
    *((int32_t*)(ptr + 8)) = (int32_t) (*payload3).len;
    *((int32_t*)(ptr + 4)) = (int32_t) (*payload3).ptr;
    
  } else {
    const wabt_string_t *payload = &(ret).val.ok;
    *((int8_t*)(ptr + 0)) = 0;
    *((int32_t*)(ptr + 8)) = (int32_t) (*payload).len;
    *((int32_t*)(ptr + 4)) = (int32_t) (*payload).ptr;
    
  }
  return ptr;
}
