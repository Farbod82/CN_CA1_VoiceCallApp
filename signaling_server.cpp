
#include "signaling_server.h"
#include <boost/asio.hpp>
#include <iostream>
#include<QDebug>
#include <QThread>


using namespace boost::asio;
using namespace std;
using ip::tcp;


vector<Client*> signalingServer::clients;

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
    // boost::asio::streambuf buffer;
    // boost::system::error_code err_code;
    // boost::asio::read_until(m_socket, buffer,'\n',err_code);
    // std::string message((std::istreambuf_iterator<char>(&buffer)), std::istreambuf_iterator<char>());
    // handle_requests(message);
    boost::asio::async_read_until(m_socket, m_buffer, "\0",
                                  [this, self](boost::system::error_code ec, std::size_t /*length*/)
                                  {
                                      if (!ec)
                                      {
                                          std::string data((std::istreambuf_iterator<char>(&m_buffer)), std::istreambuf_iterator<char>());
                                          std::cout << data << std::endl;
                                          handle_requests(data);
                                          // start();
                                      }
                                      else{
                                          cout << "oops" << endl;
                                      }
                                  });
}


void signalingServer::handle_requests(string data){
    if(strncmp(data.c_str(),"CONNECT",7) ==0){
        int dummy;
        string name = extract_command_info(data,dummy,"CONNECT");
        string address = m_socket.remote_endpoint().address().to_string();
        int port = m_socket.remote_endpoint().port();
        Client* new_client = new Client;
        new_client->port = port;
        new_client->name = name;
        new_client->address = address;
        signalingServer::clients.push_back(new_client);
    }
    else if(strncmp(data.c_str(),"CALL",4)==0){
        int response_port;
        string name = extract_command_info(data,response_port,"CALL");
        Client* reciever = find_user_by_name(name);
        string response = "RESPONSE " + reciever->address + " " + to_string(reciever->port);
        send_response(response,reciever,response_port);
    }
}
Client* signalingServer::find_user_by_name(string name){
    for (auto client: clients){
        if(client->name.compare(name) == 0){
            return client;
        }
    }
    return nullptr;
}

void signalingServer::send_response(string response,Client* reciever,int response_port){
    // boost::asio::async_write(
    //      m_socket, boost::asio::buffer(response+"\n"),
    //      [this](boost::system::error_code ec, std::size_t /*length*/) {
    //          if (ec) {
    //              std::cout << "Error sending: " << ec.message() << std::endl;
    //          }
    //          // cout <<"oops"<<endl;
    //          // write_back();
    //      });
    boost::asio::io_context io_context;

    // we need a socket and a resolver
    tcp::socket socket(io_context);
    tcp::resolver resolver(io_context);

    // now we can use connect(..)
    boost::asio::connect(socket, resolver.resolve(m_socket.remote_endpoint().address().to_string(), to_string(response_port)));
    boost::asio::write(socket,boost::asio::buffer(response));
 //   cout << "sent" << endl;
}


string signalingServer::extract_command_info(string data,int& response_port,string command){
    std::istringstream iss(data);
    std::vector<std::string> words;

    std::string word;
    while (std::getline(iss, word, ' ')) {
        words.push_back(word);
    }
    if(command.compare("CALL") == 0){
        response_port = std::stoi(words[2]);
    }
    return words[1];
}
