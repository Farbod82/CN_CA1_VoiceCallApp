#ifndef LISTNER_H
#define LISTNER_H


#include <boost/asio.hpp>
#include<QDebug>
#include <QThread>


using namespace boost::asio;
using namespace std;
using ip::tcp;


class Listner
{
public:
    Listner(boost::asio::io_context& io_context, short port);

private:
    void do_accept();

    tcp::acceptor m_acceptor;
};

#endif // LISTNER_H
