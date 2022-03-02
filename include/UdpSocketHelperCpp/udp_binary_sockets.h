#pragma once
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <queue>
#include <mutex>
using boost::asio::ip::tcp;
using boost::asio::ip::udp;

namespace udp_binary_helper
{

typedef std::shared_ptr<udp::socket> socket_ptr;

class Sender
{
protected:
  boost::asio::io_service io_service;
  boost::asio::io_context io_context;
  udp::endpoint remote_endpoint;
  udp::resolver resolver;
  socket_ptr sock;
  std::string hostname_;
  std::string port_;
public:
  Sender(std::string hostname, std::string port):
    hostname_(hostname),
    resolver(io_context),
    port_(port)
  {
    boost::system::error_code error;
    udp::resolver::iterator iter = resolver.resolve(hostname, port,error);
    assert(!error);
    remote_endpoint = *iter;

    sock=std::make_shared<udp::socket>(io_service, udp::endpoint(udp::v4(), 0));

  }

  bool sendData(const std::vector<double>& data)
  {
    try
    {
      boost::system::error_code err;
      auto sent = sock->send_to(boost::asio::buffer(data,sizeof(double)*data.size()), remote_endpoint, 0, err);
      return true;
    }
    catch (std::exception& e)
    {
      std::cerr << "Exception in thread: " << e.what() << "\n";
      return false;
    }
  }
};

class Receiver
{
protected:
  udp::endpoint local_endpoint_;
  udp::endpoint sender_endpoint_;
  boost::asio::io_service io_service_;
  socket_ptr sock_;
  std::string port_;
  bool stop_;
  size_t size_;
  std::queue<std::vector<double>> queue_;
  std::mutex mtx_;
  std::thread read_thread_;

  char buffer_[1024]={};

  void handle_receive_from(const boost::system::error_code& error,
                           size_t bytes_recvd)
  {
    if (!error && bytes_recvd > 0)
    {
      std::vector<double> data;
      data.insert(data.begin(),(double*)buffer_,(double*)buffer_+bytes_recvd/sizeof(double));
      std::lock_guard<std::mutex> lock(mtx_);
      queue_.push(data);
    }
    sock_->async_receive_from(
          boost::asio::buffer(buffer_,size_*sizeof(double)), sender_endpoint_,
          boost::bind(&Receiver::handle_receive_from, this,
                      boost::asio::placeholders::error,
                      boost::asio::placeholders::bytes_transferred));
  }

  void read()
  {
    io_service_.run();
  }



public:
  Receiver(std::string port):
    port_(port)
  {
    boost::system::error_code error;
    int port_int=std::stoi(port);
    assert(!error);

    local_endpoint_ = boost::asio::ip::udp::endpoint(
          udp::v4(), port_int);
    sock_=std::make_shared<udp::socket>(io_service_,local_endpoint_);

    stop_=false;
    sock_->async_receive_from(
          boost::asio::buffer(buffer_,size_*sizeof(double)), sender_endpoint_,
          boost::bind(&Receiver::handle_receive_from, this,
                      boost::asio::placeholders::error,
                      boost::asio::placeholders::bytes_transferred));
    read_thread_=std::thread(&Receiver::read,this);
  }
  ~Receiver()
  {
    stop_=true;
    io_service_.stop();
    if (read_thread_.joinable())
      read_thread_.join();
  }

  void setDataSize(size_t size){size_=size;}

  bool isUnreadDataAvailable()
  {
    return  (not queue_.empty());
  }

  std::vector<double> getData()
  {
    std::lock_guard<std::mutex> lock(mtx_);
    if (queue_.size()>0)
    {
      std::vector<double> data=queue_.front();
      queue_.pop();
      return data;
    }
    else
    {
      std::vector<double> data;
      return data;
    }
  }

  void clearQueue()
  {
    std::lock_guard<std::mutex> lock(mtx_);
    while (queue_.size()>0)
    {
      queue_.pop();
    }
  }

  void thread()
  {
    while (not stop_)
    {
      read();
      usleep(1000);
    }
  }

};

}

