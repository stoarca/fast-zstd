// module.exports.hello = () => 'world';

#include <node.h>
#include <zstd.h>
#include "common.h"

namespace demo {

using v8::ArrayBuffer;
using v8::BackingStore;
using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Uint8Array;
using v8::Value;

void Compress(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() > 2 || args.Length() < 1) {
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong number of arguments").ToLocalChecked()
    ));
    return;
  }

  if (args.Length() >= 1 && !args[0]->IsUint8Array()) {
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(
        isolate, "First arg must be Uint8Array: data to compress"
      ).ToLocalChecked()
    ));
    return;
  }

  if (args.Length() >= 2 && !args[1]->IsNumber()) {
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(
        isolate, "Second arg must be number: compression level"
      ).ToLocalChecked()
    ));
    return;
  }

  int compressionLevel = 1;
  if (args[1]->IsNumber()) {
    compressionLevel = args[1].As<Number>()->Value();
  }

  auto input = args[0].As<Uint8Array>();
  size_t inSize = input->Length();
  auto inBuff = input->Buffer()->GetBackingStore()->Data();
  size_t const outSize = ZSTD_compressBound(inSize);
  auto backingStore = ArrayBuffer::NewBackingStore(isolate, outSize);

  size_t const outActualSize = ZSTD_compress(
    backingStore->Data(),
    outSize,
    (uint8_t *) inBuff + input->ByteOffset(),
    inSize,
    compressionLevel
  );
  CHECK_ZSTD(outActualSize);

  args.GetReturnValue().Set(Uint8Array::New(
    ArrayBuffer::New(isolate, std::move(backingStore)),
    0,
    outActualSize
  ));
}

void Decompress(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 1) {
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong number of arguments").ToLocalChecked()
    ));
    return;
  }

  if (!args[0]->IsUint8Array()) {
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(
        isolate, "First arg must be Uint8Array"
      ).ToLocalChecked()
    ));
    return;
  }

  auto input = args[0].As<Uint8Array>();
  size_t inSize = input->Length();
  auto inBuff = input->Buffer()->GetBackingStore()->Data();
  size_t const outSize = ZSTD_getFrameContentSize(
    (uint8_t *) inBuff + input->ByteOffset(),
    inSize
  );
  CHECK_ZSTD(outSize);
  auto backingStore = ArrayBuffer::NewBackingStore(isolate, outSize);

  size_t const outActualSize = ZSTD_decompress(
    backingStore->Data(),
    outSize,
    (uint8_t *) inBuff + input->ByteOffset(),
    inSize
  );
  CHECK_ZSTD(outActualSize);

  args.GetReturnValue().Set(Uint8Array::New(
    ArrayBuffer::New(isolate, std::move(backingStore)),
    0,
    outActualSize
  ));
}

void Initialize(Local<Object> exports) {
  NODE_SET_METHOD(exports, "compress", Compress);
  NODE_SET_METHOD(exports, "decompress", Decompress);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

}  // namespace demo
