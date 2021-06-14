#![deny(warnings)]

use std::convert::Infallible;
use hyper::service::{make_service_fn, service_fn};
use hyper::{Body, Request, Response, Server};

async fn hello(_: Request<Body>) -> Result<Response<Body>, Infallible> {
	let mut response = Response::new(Body::from("Hello, World!\r\n"));
	let headers = response.headers_mut();
	headers.insert(hyper::header::CONTENT_TYPE, "text/plain; charset=UTF-8".parse().unwrap());
	headers.insert(hyper::header::SERVER, "Example".parse().unwrap());
	headers.insert(hyper::header::DATE, "Wed, 17 Apr 2013 12:00:00 GMT".parse().unwrap());
	Ok(response)
}

#[tokio::main]
pub async fn main() -> Result<(), Box<dyn std::error::Error + Send + Sync>> {
    pretty_env_logger::init();
    // For every connection, we must make a `Service` to handle all
    // incoming HTTP requests on said connection.
    let make_svc = make_service_fn(|_conn| {
        // This is the `Service` that will handle the connection.
        // `service_fn` is a helper to convert a function that
        // returns a Response into a `Service`.
        async { Ok::<_, Infallible>(service_fn(hello)) }
    });
    let addr = ([127, 0, 0, 1], 8080).into();
    let server = Server::bind(&addr).serve(make_svc);
    println!("Listening on http://{}", addr);
    server.await?;
    Ok(())
}
