use actix_web::{HttpServer, App, web};
use actix_web::HttpResponse;
use std::io;

async fn hello() -> HttpResponse {
    HttpResponse::Ok()
        .content_type("text/plain; charset=UTF-8")
        .header("Server", "Example")
        .header("Date", "Wed, 17 Apr 2013 12:00:00 GMT")
        .body("Hello, World!\r\n")
}

#[actix_rt::main]
async fn main() -> io::Result<()> {
	println!("Starting server at 127.0.0.1:8542");
	HttpServer::new(|| {
		App::new().route("/foo/bar", web::get().to(hello))
	})
	.bind("127.0.0.1:8542")?
	.run()
	.await
}
