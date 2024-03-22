#include "listner.h"
#include "signaling_server.h"





#include <boost/asio.hpp>
#include <iostream>
#include<QDebug>
#include <QThread>


using namespace boost::asio;
using namespace std;
using ip::tcp;


Listner::Listner(boost::asio::io_context& io_context, short port)
    : m_acceptor(io_context, tcp::endpoint(tcp::v4(), port))
{
    do_accept();
}

void Listner::do_accept()
{
    m_acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec)
        {
            std::cout << "creating session on: "
                      << socket.remote_endpoint().address().to_string()
                      << ":" << socket.remote_endpoint().port() << '\n';
            std::make_shared<signalingServer>(std::move(socket))->run();
        }
        else
        {
            std::cout << "error: " << ec.message() << std::endl;
        }
        do_accept();
    });
}
