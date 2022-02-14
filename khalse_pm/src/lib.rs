extern crate libc;
use lettre::transport::smtp::authentication::Credentials;
use lettre::{Message, SmtpTransport, Transport};
use libc::{c_char, c_int};
use rand::Rng;
use std::ffi::{CStr};


type CSTR = *const c_char;

struct Smtp {
    id: i32,
    agent: SmtpTransport,
}

static mut CONNECTIONS: Vec<Smtp> = Vec::new();


#[no_mangle]
pub unsafe extern "C" fn connect_smtp(host: CSTR, username: CSTR, password: CSTR) -> i32 {
    println!("Rust got host, username, port as following: {}, {}, {}", CStr::from_ptr(host).to_str().unwrap(), CStr::from_ptr(username).to_str().unwrap(), CStr::from_ptr(password).to_str().unwrap());

    let r_host = String::from(CStr::from_ptr(host).to_str().unwrap());
    let r_username = String::from(CStr::from_ptr(username).to_str().unwrap());
    let r_password = String::from(CStr::from_ptr(password).to_str().unwrap());

    let creds = Credentials::new(r_username, r_password);

    let smtp_transport = SmtpTransport::relay(r_host.as_str()).unwrap()
        .credentials(creds)
        .build();

    let id: i32 = rand::thread_rng().gen_range(1..1_000_000);

    CONNECTIONS.push(Smtp{id, agent: smtp_transport});

    id
}

#[no_mangle]
pub unsafe extern "C" fn close_connection(conn_id: c_int) -> bool {
    for (id, element) in CONNECTIONS.iter().enumerate() {
        if element.id == conn_id {
            CONNECTIONS.remove(id);

            return true;
        }
    }
    false
}

/// RIP Error handling!
/// Error handling must be done properly
#[no_mangle]
pub unsafe extern "C" fn send_mail(conn_id: c_int, sender: CSTR, to: CSTR, subject: CSTR, body: CSTR) -> bool {
    let mut conn: Option<&Smtp> = None;

    for (_, element) in CONNECTIONS.iter().enumerate() {
        if element.id == conn_id {
            conn = Some(element);
        }
    }
    if let None = conn {
        return false;
    }

    let conn = match conn {
        Some(c) => c,
        None => {
            return false;
        },
    };

    let sender = String::from(CStr::from_ptr(sender).to_str().unwrap());
    let to = String::from(CStr::from_ptr(to).to_str().unwrap());
    let subject = String::from(CStr::from_ptr(subject).to_str().unwrap());
    let body = String::from(CStr::from_ptr(body).to_str().unwrap());

    let email = Message::builder()
        .from(sender.parse().unwrap())
        .to(to.parse().unwrap())
        .subject(subject)
        .reply_to(to.parse().unwrap())
        .body(body);
    let email = match email {
        Ok(e) => e,
        Err(_) => {
            return false;
        }
    };

    match conn.agent.send(&email) {
        Ok(_) => true,
        Err(_) => false,
    }
}

