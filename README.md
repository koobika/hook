# Hook

Header only, and cross-platform, high performance C++ library offering for a wide and complete set of modules (http server/client, json/yaml encoder/decoder, filesystem management/event tracker, multi-process/multi-thread concurrency primitives, console processor, and many more...). Build demanding applications in an easy and modern way!

## Official documentation

You can find official hook api doc under [docs](./docs) folder, and visit it online at [hook documentation site](https://koobika.github.io/hook/).

Also, we made a talk in [Italian CPP Con 2021](), so you can watch the presentation here:

[![ITCPPCON21](https://img.youtube.com/vi/kVqmbAaMz_4/0.jpg)](https://www.youtube.com/watch?v=kVqmbAaMz_4)

## Ok, i'm conviced, how to use it ?

It's a header-only library. Just add the 'include' folder to your compiler's includes-search folder and that's all!

NOTE: Soon, we will release hook, with which you will be able to manage dependencies using a command line tool.

## Current Modules

* [HTTP Server](doc/modules/httpserver.md)
* [JSON](doc/modules/json.md)
* [Encoding](doc/modules/encoding.md)

## A quick (but powerful) sample

With hook, you have the power of C++ coding like 2021 languages. Take a look to this example, using our [http server](https://koobika.github.io/hook/classkoobika_1_1hook_1_1network_1_1protocol_1_1http_1_1_server.html) to create a REST endpoint with Url parameters that compose a JSON object to get it back to the client caller.

```cpp
//! [Example]
#include "network/protocol/http/server_builder.h"
#include "network/protocol/http/response_builder.h"
#include "structured/json/value.h"

using namespace koobika::hook::network::protocol::http;
using namespace koobika::hook::structured;

int main() {
  try {
    auto server = ServerBuilder().Build();
    server->Handle("/{level}/{resource}", [](const Request& req, Response& res,
                                             const Parameters& parameters) {
      // Here, we use 'parameters' variable to find needed values!
      // Since we're only interested on /foo/bar URI, let's check that
      // the incoming URL is fulfilling the requirements..
      auto level = parameters.find("level");
      auto resource = parameters.find("resource");
      if (level != parameters.end() && resource != parameters.end()) {
        ResponseBuilder::Build(res,
                               json::Object{{"{level}", level->second},
                                            {"{resource}", resource->second}},
                               constants::Mime::kJSON)
            .Ok_200();
      } else {
        res.Body.Write("Resource Uri not supported!");
        res.Forbidden_403();
      }
    });
    server->Start("8080");
    return getchar();
  } catch (const std::exception& exception) {
    // ((Error)) -> while performing setup!
    std::cout << exception.what() << std::endl;
    return -1;
  }
}
//! [Example]

```
Take a look to our [samples](./src/examples) folder for more cool stuff.

## @

Koobika Team