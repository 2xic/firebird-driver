#include <node.h>
#include "database/Firebird.h"

void query(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Firebird *cli = new Firebird();
    cli->Connect();
    cli->Query();
    cli->Disconnect();

    v8::Isolate* isolate = args.GetIsolate();
    args.GetReturnValue().Set(v8::String::NewFromUtf8(
        isolate, "world").ToLocalChecked());
}

void Initialize(v8::Local<v8::Object> exports) {
  NODE_SET_METHOD(exports, "query", query);
}

NODE_MODULE(module_name, Initialize);
