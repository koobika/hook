#[global_allocator]
static GLOBAL: mimalloc::MiMalloc = mimalloc::MiMalloc;

use bytes::Bytes;
use ntex::http::header::{HeaderValue, CONTENT_TYPE, SERVER};
use ntex::{http, web};
use yarte::Serialize;

mod utils;

#[derive(Serialize)]
pub struct Message {
    pub message: &'static str,
}

#[web::get("/plaintext")]
async fn plaintext() -> web::HttpResponse {
    web::HttpResponse::Ok()
        .header(SERVER, HeaderValue::from_static("Example"))
        .header(CONTENT_TYPE, HeaderValue::from_static("text/plain; charset=UTF-8"))
        .body(Bytes::from_static(b"Hello, World!\r\n"))
}

#[ntex::main]
async fn main() -> std::io::Result<()> {
    println!("Started http server: 127.0.0.1:8080");
    ntex::server::build()
        .backlog(1024)
        .bind("techempower", "0.0.0.0:8080", || {
            http::HttpService::build()
                .keep_alive(http::KeepAlive::Os)
                .client_timeout(0)
                .disconnect_timeout(0)
                .buffer_params(65535, 65535, 1024)
                .h1(web::App::new()
                    .service(plaintext)
                    .with_config(web::dev::AppConfig::default()))
                .tcp()
        })?
        .start()
        .await
}
