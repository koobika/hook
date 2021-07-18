#include "./cpp-httplib/httplib.h"

using namespace httplib;

int main(void) {
  Server svr;

  svr.Get("/plaintext", [](const Request & /*req*/, Response &res) {
      res.set_header("Server", "Example");
      res.set_header("Date", "Wed, 17 Apr 2013 12:00:00 GMT");
      res.set_header("Content-Type", "text/plain; charset=UTF-8");
      res.set_content("Hello World!\r\n", "text/plain");
  });

  svr.listen("localhost", 8080);
}