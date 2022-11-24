import { readFile } from 'fs/promises';
import { WABT, WasmFeature } from './input.js';

let contents = await readFile(new URL('../../out/libwabt.wasm', import.meta.url));
let wabt = new WABT();
await wabt.instantiate(contents, {
    "wasi_snapshot_preview1": {
        "fd_close": function() {throw new Error("fd_close")},
        "fd_seek": function() {throw new Error("fd_seek")},
        "fd_write": function() {throw new Error("fd_write")},
        "environ_get": function() {throw new Error("environ_get")},
        "environ_sizes_get": function() {throw new Error("environ_sizes_get")},
        "fd_fdstat_get": function() {throw new Error("fd_fdstat_get")},
        "proc_exit": function() {throw new Error("proc_exit")},
    }
});

function wat2wasm(contents) {
    console.log(`trying ${contents}`);
    try {
        let result = wabt.wat2wasm(contents, WasmFeature.MutableGlobals);
        console.log(result)    }
    catch (e) {console.log("error", e)}
}

export const WASM_FEATURE_EXCEPTIONS = 1;
export const WASM_FEATURE_MUTABLE_GLOBALS = 2;
export const WASM_FEATURE_SAT_FLOAT_TO_INT = 4;
export const WASM_FEATURE_SIGN_EXTENSION = 8;
export const WASM_FEATURE_SIMD = 16;
export const WASM_FEATURE_THREADS = 32;
export const WASM_FEATURE_MULTI_VALUE = 64;
export const WASM_FEATURE_TAIL_CALL = 128;
export const WASM_FEATURE_BULK_MEMORY = 256;
export const WASM_FEATURE_REFERENCE_TYPES = 512;
export const WASM_FEATURE_ANNOTATIONS = 1024;
export const WASM_FEATURE_GC = 2048;


function wasm2wat(binary) {
    // console.log(`trying ${binary}`);
    try {
        let result = wabt.wasm2wat(binary, WASM_FEATURE_EXCEPTIONS | WASM_FEATURE_MUTABLE_GLOBALS | WASM_FEATURE_SAT_FLOAT_TO_INT | WASM_FEATURE_SIGN_EXTENSION | WASM_FEATURE_SIMD | WASM_FEATURE_THREADS | WASM_FEATURE_MULTI_VALUE | WASM_FEATURE_BULK_MEMORY | WASM_FEATURE_REFERENCE_TYPES | WASM_FEATURE_ANNOTATIONS);
        console.log(result.val)
    }
    catch (e) {console.log("error", e)}
}


// wat2wasm("(module)");
// wat2wasm("xyz");
// wasm2wat(new Uint8Array([
//     0, 97, 115, 109,   1,   0,   0,   0,   0,
//     8,  4, 110,  97, 109, 101,   2,   1,   0,
//     0,  9,   7, 108, 105, 110, 107, 105, 110,
//   103,  2
// ]))

let wabtFile = await readFile(new URL('../wabt.wasm', import.meta.url));
console.log("Trying wabt");
wasm2wat(wabtFile)
