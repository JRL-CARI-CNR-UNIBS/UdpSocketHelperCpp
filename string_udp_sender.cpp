//
// blocking_tcp_echo_client.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <signal.h>
#include "UdpSocketHelperCpp/udp_string_sockets.h"

bool stop=false;

void my_handler(int s)
{
  printf("Caught signal %d\n",s);
  stop=true;
}



int main(int argc, char* argv[])
{
  signal (SIGINT,my_handler);
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: blocking_tcp_echo_client <port>\n";
      return 1;
    }

    std::string host_name="127.0.0.1";
    std::cout << "host name : " <<host_name<<std::endl;
    std::string port =argv[1];
    std::cout << "port : " <<std::atoi(port.c_str())<<std::endl;

    udp_string_helper::Sender string_sender(host_name,port);

    int idx=0;

    while (not stop)
    {
      std::string str="test_"+std::to_string(idx++)+"\n";
      string_sender.sendString(str);
      sleep(2);
    }

  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
