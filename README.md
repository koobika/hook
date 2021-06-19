# Hook

Header only, and cross-platform, high performance C++ library offering for a wide and complete set of modules (http server/client, json/yaml encoder/decoder, filesystem management/event tracker, multi-process/multi-thread concurrency primitives, console processor, and many more...). Build demanding applications in an easy and modern way!

# Modules

## Http Server 
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

## Build

### Linux pre-requisites

In order to build it on linux, you need to meet some pre-requisites:

* Install [microsoft package](https://docs.microsoft.com/en-us/windows-server/administration/linux-package-repository-for-microsoft-software) for your distro
* Install [Powershell](https://docs.microsoft.com/es-es/powershell/scripting/install/installing-powershell-core-on-linux?view=powershell-7.1)
* Install [.net 5 sdk](https://docs.microsoft.com/es-es/dotnet/core/install/linux)

### Build it!
