#include "signaling_server.h"
#include <boost/asio.hpp>
#include <iostream>
#include<QDebug>
#include <QThread>


using namespace boost::asio;
using namespace std;
using ip::tcp;




signalingServer::signalingServer(tcp::socket socket)
    : m_socket(std::move(socket))
{
}

void signalingServer::run()
{
    wait_for_request();
}

void signalingServer::wait_for_request()
{
    auto self(shared_from_this());

    boost::asio::async_read_until(m_socket, m_buffer, "\0",
                                  [this, self](boost::system::error_code ec, std::size_t /*length*/)
                                  {
                                      if (!ec)
                                      {
                                          std::string data((std::istreambuf_iterator<char>(&m_buffer)), std::istreambuf_iterator<char>());
                                          std::cout << data << std::endl;
                                          wait_for_request();
                                      }
                                  });
}











