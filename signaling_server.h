#ifndef SIGNALING_SERVER_H
#define SIGNALING_SERVER_H

#include <boost/asio.hpp>
#include<QDebug>
#include <QThread>


using namespace boost::asio;
using namespace std;
using ip::tcp;

class signalingServer : public std::enable_shared_from_this<signalingServer>
{
public:
    signalingServer(tcp::socket socket);

    void run();

    void handle_requests(std::string request_data);
private:
    void wait_for_request();

    tcp::socket m_socket;
    boost::asio::streambuf m_buffer;
};

#endif // SIGNALING_SERVER_H
