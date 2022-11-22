#!/bin/sh


wasmer run wasmer/wai-bindgen-cli --dir=. -- c --export wabt.wit 

# Note: WABT has set up the include path so all #includes need to be relative to
# the project root
sed -i 's|#include <bindings.h>|#include "src/embedding/bindings.h"|g' bindings.c
