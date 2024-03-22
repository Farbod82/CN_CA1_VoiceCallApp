

#include <boost/asio.hpp>
#include <string>
using boost::asio::ip::tcp;



void runClient2()
{
    using boost::asio::ip::tcp;
    boost::asio::io_context io_context;

    // we need a socket and a resolver
    tcp::socket socket(io_context);
    tcp::resolver resolver(io_context);

    // now we can use connect(..)
    boost::asio::connect(socket, resolver.resolve("192.168.66.161", "8080"));

    // and use write(..) to send some data which is here just a string
    std::string data{"Hello ..."};
    auto result = boost::asio::write(socket, boost::asio::buffer(data));
    // the result represents the size of the sent data
    // and close the connection now
    boost::system::error_code ec;
    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    socket.close();
}
