#include <cstddef>

#include <algorithm>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

#include "wabt/apply-names.h"
#include "wabt/binary-reader-ir.h"
#include "wabt/binary-reader.h"
#include "wabt/binary-writer-spec.h"
#include "wabt/binary-writer.h"
#include "wabt/common.h"
#include "wabt/error-formatter.h"
#include "wabt/feature.h"
#include "wabt/filenames.h"
#include "wabt/generate-names.h"
#include "wabt/ir.h"
#include "wabt/stream.h"
#include "wabt/validator.h"
#include "wabt/wast-lexer.h"
#include "wabt/wast-parser.h"
#include "wabt/wat-writer.h"

#include "./wabt.h"

void bindings_wat2wasm(bindings_string_t* wat,
                       bindings_wasm_feature_t features,
                       bindings_expected_list_u8_string_t* ret0) {
  wabt::Errors* errors = new wabt::Errors();
  std::unique_ptr<wabt::WastLexer> lexer =
      wabt::WastLexer::CreateBufferLexer("", wat->ptr, wat->len, errors);
  wabt::Features* wabtfeatures = new wabt::Features();

  wabt::WastLexer* finalLexer = lexer.release();
  wabt::WastParseOptions options(*wabtfeatures);
  std::unique_ptr<wabt::Module> module;
  wabt::Result parseResult =
      wabt::ParseWatModule(finalLexer, &module, errors, &options);

  if (parseResult != wabt::Result::Ok) {
    // Line finder triggers bus error in Node
    // auto line_finder = lexer->MakeLineFinder();
    std::string string_result =
        FormatErrorsToString(*errors, wabt::Location::Type::Binary);
    // std::string string_result = FormatErrorsToString(
    //     *errors, wabt::Location::Type::Text, line_finder.get());
    ret0->is_err = true;
    bindings_string_set(&ret0->val.err, string_result.c_str());
    return;
  }
  wabt::Module* moduleFinal = module.get();

  bool log = true;
  wabt::MemoryStream log_stream;
  wabt::WriteBinaryOptions writeoptions;
  writeoptions.canonicalize_lebs = 1;
  writeoptions.relocatable = 1;
  writeoptions.write_debug_names = 1;

  wabt::MemoryStream stream(log ? &log_stream : nullptr);
  wabt::Result result = WriteBinaryModule(&stream, moduleFinal, writeoptions);
  if (result == wabt::Result::Ok) {
    std::unique_ptr<wabt::OutputBuffer> output_buffer;
    output_buffer = stream.ReleaseOutputBuffer();
    ret0->is_err = false;
    bindings_list_u8_t okval;
    wabt::OutputBuffer* realOut = output_buffer.release();
    okval.ptr = realOut->data.data();
    okval.len = realOut->data.size();
    ret0->val.ok = okval;
    return;
  } else {
    ret0->is_err = true;
    return;
  }
}

void bindings_wasm2wat(bindings_list_u8_t* wasm,
                       bindings_wasm_feature_t features,
                       bindings_expected_string_string_t* ret0) {
  wabt::ReadBinaryOptions options;
  wabt::Features* wabtfeatures;
  wabtfeatures = new wabt::Features();
  options.features = *wabtfeatures;
  // options.read_debug_names = read_debug_names;

  wabt::Module* module = new wabt::Module();
  // TODO(binji): Pass through from wabt_read_binary parameter.
  const char* filename = "<binary>";
  wabt::Result parseResult;
  wabt::Errors* errors = new wabt::Errors();
  parseResult = wabt::ReadBinaryIr(filename, wasm->ptr, wasm->len, options,
                                   errors, module);

  if (parseResult != wabt::Result::Ok) {
    std::string string_result =
        FormatErrorsToString(*errors, wabt::Location::Type::Binary);
    ret0->is_err = true;
    bindings_string_set(&ret0->val.err, string_result.c_str());
    return;
  }

  wabt::WriteWatOptions watoptions;
  watoptions.fold_exprs = false;
  watoptions.inline_export = false;

  wabt::MemoryStream stream;
  wabt::Result watResult;
  watResult = WriteWat(&stream, module, watoptions);
  if (watResult == wabt::Result::Ok) {
    bindings_string_t okval;
    std::unique_ptr<wabt::OutputBuffer> output_buffer;
    output_buffer = stream.ReleaseOutputBuffer();
    wabt::OutputBuffer* realOut = output_buffer.release();
    std::string string_result(realOut->data.begin(), realOut->data.end());
    bindings_string_set(&okval, string_result.c_str());
    ret0->is_err = false;
    ret0->val.ok = okval;
    return;
  } else {
    ret0->is_err = true;
    return;
  }
}
