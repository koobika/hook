use actix_web::{HttpServer, App, web, Responder};
use std::io;

async fn status() -> impl Responder {
	"Hello, World!\r\n"
}

#[actix_rt::main]
async fn main() -> io::Result<()> {
	println!("Starting server at 127.0.0.1:8542");
	HttpServer::new(|| {
		App::new().route("/foo/bar", web::get().to(status))
	})
	.bind("127.0.0.1:8542")?
	.run()
	.await
}
