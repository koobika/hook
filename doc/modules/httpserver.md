# HttpServer

Http server default provided implementation module using socket-based TCP/IP transport, HTTP1.1 request decoder and the default provided router.

Offering for the following functionalities:

* Leading in performance, even in highly demanding scenarios (e.g.: thousand of simultaneous clients)
* Full routing capabilities (both string and regular-expressions based routes)
* Controllers, allowing to group any set of related end-points within a dedicated class context
* Authentication (NoAuth, Basic, Api-Token and Bearer-Token currently supported by default). Also, making use of an open design concept that will allow any developer to add new authentication methods in an easy and standardized way
* Body encoders/decoders, allowing an easy way to read/write from/to body parts using serialized/raw content
* Static file server provided by default
* Transfer-encoding support (currently, only 'chunked' mode supported)
* Multipart/form (content-type) support
* Stream based (memory/file) data ingestion, avoiding out-of-memory scenarios due to big buffers management scenarios
* Hyper fast message decoding routines based on asynchronous dispatching
* Fully configurable via builder (e.g.: concurrency levels, maximum number of connections, etc...)
* Very low memory footprint (currently, lower than in any other available library)
* Exception safe design

## Usage

It's a header-only library. Just add the 'include' folder to your compiler's includes-search folder and that's all!

## Examples (Where to start)

* [http_server_hello_world.cpp](src/examples/network/http_server_hello_world.cpp): Hello world example introducing a very first (and simple) example. Try it!
* [http_server_builder.cpp](src/examples/network/http_server_builder.cpp): Using the builder to configure your server instance.
* [http_server_handler_shortcut.cpp](src/examples/network/http_server_handler_shortcut.cpp): Customize your end-points using explicit (GET|PUT|...) registration.
* [http_server_response_builder.cpp](src/examples/network/http_server_response_builder.cpp): Build complex payload using the built-in response builder!
* [http_server_response_builder_writer_class.cpp](src/examples/network/http_server_response_builder_writer_class.cpp):  Provide for your own response builder writers (class based).
* [http_server_response_builder_writer_object.cpp](src/examples/network/http_server_response_builder_writer_object.cpp):  Provide for your own response builder writers (object based).
* [http_server_response_builder_serializable_class.cpp](src/examples/network/http_server_response_builder_serializable_class.cpp): Provide for your own serializable content (class based).
* [http_server_response_builder_serializable.cpp](src/examples/network/http_server_response_builder_serializable.cpp): Provide for your own serializable content (object based).
* [http_server_response_builder_json.cpp](src/examples/network/http_server_response_builder_json.cpp): Writing Json stuff to the response using the builder.
* [http_server_response_builder_transfer_encoding.cpp](src/examples/network/http_server_response_builder_transfer_encoding.cpp):  Using 'transfer-encoding' while writing data.
* [http_server_raw_response.cpp](src/examples/network/http_server_raw_response.cpp):  Writing Raw data as response.
* [http_server_extended_routing_parameters.cpp](src/examples/network/http_server_extended_routing_parameters.cpp):  Creating/Accessing parameter-driven routes handlers.
* [http_server_controller.cpp](src/examples/network/http_server_controller.cpp): Create complex REST APIs in a simple way!
* [http_server_auth_basic.cpp](src/examples/network/http_server_auth_basic.cpp): Add 'basic-authorization' to your server.
* [http_server_auth_api_key.cpp](src/examples/network/http_server_auth_api_key.cpp):  Add 'api-token-authorization' to your server.
* [http_server_auth_basic_custom_checker.cpp](src/examples/network/http_server_auth_basic_custom_checker.cpp): Add 'basic-authorization' to your server using a custom checker.
* [http_server_auth_basic_controller.cpp](src/examples/network/http_server_auth_basic_controller.cpp): Add 'basic-authorization' to your controller.
* [http_server_auth_basic_controller_custom_checker.cpp](src/examples/network/http_server_auth_basic_controller_custom_checker.cpp): Add 'basic-authorization' to your controller using a custom checker.
* [http_server_auth_custom.cpp](src/examples/network/http_server_auth_custom.cpp): Add your own customized authorization mechanism to your server.
* [http_server_static_files_server.cpp](src/examples/network/http_server_static_files_server.cpp): Use the built-in 'statics-file-server' controller!