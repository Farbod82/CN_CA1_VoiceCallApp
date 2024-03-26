
#ifndef SIGNALING_SERVER_H
#define SIGNALING_SERVER_H

#include <boost/asio.hpp>
#include<QDebug>
#include <QThread>


using namespace boost::asio;
using namespace std;
using ip::tcp;




struct Client{
    string name;
    string address;
    int port;
};

class signalingServer : public std::enable_shared_from_this<signalingServer>
{
public:
    signalingServer(tcp::socket socket);
    void run();
    void handle_requests(std::string request_data);
    Client *find_user_by_name(string name);
    void wait_for_request();
    void send_response(string response, Client *reciever, int response_port);
    string extract_command_info(string data, int &response_port, string command);
private:
    static vector<Client*> clients;
    tcp::socket m_socket;
    boost::asio::streambuf m_buffer;
};

#endif // SIGNALING_SERVER_H
