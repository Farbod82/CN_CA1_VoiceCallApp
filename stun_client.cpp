// #include <QCoreApplication>
// #include <QThread>
// #include <QDebug>
// #include <boost/asio.hpp>
// #include <array>

// using boost::asio::ip::udp;

// const std::array<unsigned char, 20> request_template = {
//     0x00, 0x01,             // STUN message type: Binding Request
//     0x00, 0x00,             // Message length: 20 bytes - 20 bytes header
//     0x21, 0x12, 0xA4, 0x42, // Magic cookie
//     // 8-19: Transaction ID (variable part, can be any value)
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
// };



// void runClient()
// {
//     boost::asio::io_context io_context;

//     udp::socket socket(io_context, udp::endpoint(udp::v4(), 0));
//     udp::resolver resolver(io_context);
//     udp::resolver::results_type endpoints = resolver.resolve("192.168.100.161", "8080");

//     const std::array<unsigned char, 20>& request = request_template;
//     socket.send_to(boost::asio::buffer(request), *endpoints.begin());

//     std::array<unsigned char, 32> response_buffer;
//     udp::endpoint sender_endpoint;

//     size_t response_length = socket.receive_from(
//         boost::asio::buffer(response_buffer), sender_endpoint);

//     std::string publicIP;
//     if (response_length >= 8) {
//         unsigned char addressFamily = response_buffer[25];
//         if (addressFamily == 0x01) { // IPv4 address
//             unsigned char ip1 = response_buffer[28] ^ 0x21;
//             unsigned char ip2 = response_buffer[29] ^ 0x12;
//             unsigned char ip3 = response_buffer[30] ^ 0xA4;
//             unsigned char ip4 = response_buffer[31] ^ 0x42;
//             publicIP = std::to_string(ip1) + '.' +
//                        std::to_string(ip2) + '.' +
//                        std::to_string(ip3) + '.' +
//                        std::to_string(ip4);
//         } else if (addressFamily == 0x02) { // IPv6 address
//             // Extract IPv6 address from the response if needed
//             // ...
//         }
//     }
//     qDebug() << publicIP;

//     io_context.run();
// }
