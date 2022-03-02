# TcpHelper

REWRITE

# Table of Contents
1. [Sender](#Python)
2. [Receiver](#c++)


## C++
Written in C++11, it requires __Boost.asio__

namespace __tcp_helper__

### TcpClient
Client for receiving strings. It runs in a separated thread, new strings are queued in FIFO order.

Usage:

- Constructor:
  * host_name: hostname of the TCP Server
  * port: port of the TCP connnection
  * stop_flag: a boolean value, the thread stops if it becomes true.

```
TcpClient::TcpClient(host_name,port,stop_flag);
```

You need to manually start the thread
```
tcp_helper::TcpClient string_rec(host_name,port,stop);
std::thread tcp_thread(&tcp_helper::TcpClient::thread,&string_rec);
```

- Check if a new string is available:
```
bool TcpClient::isUnreadStringAvailable()
```


- get received string (queued in FIFO order). Return "" if there are no strings.
```
std::string TcpClient::getString()
```


- get the last received string and clear the queue. Return "" if there are no strings.
```
std::string TcpClient::getLastStringAndClearQueue()
```

### TcpServer
Server for sending strings. It runs in a separated thread, new strings are queued in FIFO order.
Usage:

- Constructor:
  * host_name: hostname of the TCP Server
  * port: port of the TCP connnection
  * stop_flag: a boolean value, the thread stops if it becomes true.

```
TcpServer::TcpServer(host_name,port,stop_flag);
```

You need to manually start the thread
```
tcp_helper::TcpServer string_rec(host_name,port,stop);
std::thread tcp_thread(&tcp_helper::TcpClient::thread,&string_rec);
```

- Check if there are no strings in queue:
```
bool TcpServer::hasEmptyQueue()
```


- append string to the queue (queued in FIFO order).
```
void sendString(const std::string& str)
```
