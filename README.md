# libfirebase

Libfirebase is a C library developed to speed up software development of Linux applications that depend on Google's firebase: https://firebase.google.com/.
At the moment the library supports:
- firebase cloud messaging - send: Read more about fcm at https://firebase.google.com/docs/cloud-messaging/. You can find a sample fcm client here: https://github.com/GoogleChrome/samples/tree/gh-pages/push-messaging-and-notifications

The library has Linux-specific functions separate as ENV to allow multiple environments (ex: Windows / No OS).

## Library API
int libfirebase_init(const uint8_t log_enabled, const char *log_file);
	Initializes the library

	Params:
	- enable logging (0 - not enabled, 1 - enabled)
	- log file path or NULL if log file is stdout

	Returns:
	- 0 on success
	- -1 on error

struct libfirebase_response_t *libfirebase_fcm_send(const char *endpoint, const char *message, const char *p256,
                                                    const char *auth, const char *api_key);
	Sends a string via FCM

	Params:
	- client endpoint as received from the cloud messaging server: string
	- message to send: string
	- client p256 key: string
	- client secret authentication token: string
	- api key for sending or NULL if it is not required

Returns:
	- HTTP response if successful: struct libfirebase_response_t with header, body and http code
	- NULL on error

struct libfirebase_response_t *libfirebase_fcm_send_bytes(const char *endpoint,
                                    const unsigned char *message, const unsigned int message_len,
                                    const char *p256, const char *auth, const char *api_key);
	Sends a binary message via FCM

	Params:
	- client endpoint as received from the cloud messaging server: string
	- message to send: byte array
	- the number of bytes in the message: unsigned int
	- client p256 key: string
	- client secret authentication token: string
	- api key for sending or NULL if it is not required

	Returns:
	- HTTP response if successful: struct libfirebase_response_t with header, body and http code
	- NULL on error

void libfirebase_free_response(struct libfirebase_response_t *resp);
	Frees the message that was returned by fcm send functions

Params:
- the libfirebase_response_t structure returned by libfirebase_fcm_send or libfirebase_fcm_send_bytes

## Dependencies

The library depends on openssl (libssl-dev) and libpthread.

## Installation

1. Download the source:<br />
	```bash
	$ git clone https://github.com/FlorinPetriuc/libfirebase.git
	$ cd libfirebase
	```

2. Compile library:<br />
	```bash
	$ make
	```

3. Compile unit tests
	```bash
	$ make unit_tests
	```

4. Run unit tests
	```bash
	$ cd build/test
	$ ./firebase_test
	```
	firebase_test must exit with the message: [INFO]tests passed

5. Install:<br />
	```bash
	$ cd ../..
	$ sudo make install
	```

## Usage

Include <b>libfirebase.h</b> in your aplication and link to <b>-lfirebase</b>.</br>
Initialize the library use the api calls to send messages. Follow the examples.

## Examples

1. <b>fcm_send_example</b>: Sends a message via Firebase cloud messaging to a client

	```bash
	$ cd samples/C
	$ make fcm_send_example
	$ ./fcm_send_example
	```

	```bash
	$ cd samples/Python
	$ python3 fcm_send_example.py
	```