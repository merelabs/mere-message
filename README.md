![CI](https://github.com/merelabs/mere-message-lib/workflows/CI/badge.svg)
# mere-message-lib
`mere-message` is a simple messaging mechanism to share information and comunnication between processes. The primary implementation is based on posix shared memory, semaphore and unix signaling mechanism to let each other know about the changes. The library also used qt's signal/slot mechanism.

## Component
There are two componnets available for processes, a) Server, b) Client; A process can be a server or a client;

### Server
A process should create a messaging server with a name -mms://unique-server-of-the-server, that client will use to connect and wait for clients to join there. 

A simple code snippet to start a server -
```
  // create the named server`
  Mere::Message::Server *server = new Mere::Message::Server("mms://lets-talk");
  
  // listen to the event of message coming
  
  // you can grab the whole message structure and/or just the message
  // grab the whole message structure
  connect(server, SIGNAL(message(const Mere::Message::Message &)), 
          this, SLOT(message(const Mere::Message::Message &)));
  
  // just the message
  connect(m_server, SIGNAL(message(const std::string &)), this, SLOT(message(const std::string &)));

  // start the server - that's al
  server->start();
```

A sample [server](https://github.com/merelabs/mere-message/tree/master/samples/server) implementation is shown in the [samples](https://github.com/merelabs/mere-message/tree/master/samples) folder.

### Client
A process that joined with a named server indentified as custom shmece uri like mms://unique-server-of-the-server.
A simple code snippet for a cient ot connect to the start -
```
  // create a client
  Mere::Message::Client client = new Mere::Message::Client("mms://lets-talk");
  
  // listen to the event of message coming
  
  // you can grab the whole message structure and/or just the message
  // grab the whole message structure
  connect(client, SIGNAL(message(const Mere::Message::Message &)), this, SLOT(message(const Mere::Message::Message &)));
  
  // just the message
  connect(client, SIGNAL(message(const std::string &)), this, SLOT(message(const std::string &)));

  // lets join - that's all
  client->join();

  // say hello
  client->send("Hi, How are you?");
```
A sample [client](https://github.com/merelabs/mere-message/tree/master/samples/client) implementation is shown in the [samples](https://github.com/merelabs/mere-message/tree/master/samples) folder.


## Build

```shell
git clone https://github.com/merelabs/mere-message-lib.git
cd mere-message-lib
qmake
make
```

## License
All the code in this repository is licensed under a [BSD-2-Clause License](LICENSE).
