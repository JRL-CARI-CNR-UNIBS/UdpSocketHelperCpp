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
#include "UdpSocketHelperCpp/udp_binary_sockets.h"

using boost::asio::ip::tcp;

enum { max_length = 1024 };
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

    udp_binary_helper::Sender sender(host_name,port);

    double idx=0.0;
    while (not stop)
    {
      std::vector<double> data(2);
      data.at(0)=idx+1;
      data.at(1)=-data.at(0);
      if (not sender.sendData(data))
        std::cerr << "unable to send data" <<std::endl;

      std::cout << "sent = ";
      for (double d: data)
      {
        std::cout << d << ", ";
      }
      std::cout << std::endl;
      idx+=0.1;
      usleep(1000);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
