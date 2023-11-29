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
#include "UdpSocketHelperCpp/udp_string_sockets.h"

using boost::asio::ip::tcp;


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
    if (argc != 3)
    {
      std::cerr << "Usage: blocking_tcp_echo_client <port> size\n";
      return 1;
    }

    std::string port =argv[1];
    std::cout << "port : " <<std::atoi(port.c_str())<<std::endl;

    std::string size =argv[2];
    std::cout << "size : " <<std::atoi(port.c_str())<<std::endl;

    udp_binary_helper::Receiver receiver(port);
    receiver.setDataSize(std::atoi(port.c_str()));

    while (not stop)
    {
      if (receiver.isUnreadDataAvailable())
      {
        std::vector<double> data=receiver.getData();
        std::cout << "received = ";
        for (double d: data)
        {
          std::cout << d << ", ";
        }
        std::cout << std::endl;
      }
      else
      {
//        std::cout << "..." <<std::endl;
      }
      usleep(200);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
