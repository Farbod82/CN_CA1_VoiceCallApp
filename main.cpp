// #include "audiorecorder.h"
#include "mainwindow.h"



// #include "test_tcp.cpp"
#include "test_client.cpp"
#include "listner.h"
#include <QApplication>
#include <QAudioFormat>
#include <QAudioInput>
#include <QThread>



void runServer2(){
    boost::asio::io_context io_context;
    // we'll just use an arbitrary port here
    Listner s(io_context, 8080);
    // and we run until our server is alive
    io_context.run();
}
void runClient2()
{
    using boost::asio::ip::tcp;
    boost::asio::io_context io_context;

    // we need a socket and a resolver
    tcp::socket socket(io_context);
    tcp::resolver resolver(io_context);

    // now we can use connect(..)
    boost::asio::connect(socket, resolver.resolve("localhost", "8080"));

    // and use write(..) to send some data which is here just a string
    std::string data{"CONNECT Fari"};
    auto result = boost::asio::write(socket, boost::asio::buffer(data));
    // boost::asio::streambuf buffer;
    // boost::system::error_code err_code;
    // here I need to read only first 16 bytes
    // boost::asio::read(socket, buffer, err_code);
    // std::string message((std::istreambuf_iterator<char>(&buffer)), std::istreambuf_iterator<char>());
    // std::cout << data << std::endl;
    // std::cout << "response: " << message<<std::endl;
    // std::string resp((std::istreambuf_iterator<char>(&buff)), std::istreambuf_iterator<char>());
    // std::cout << resp << std::endl;
    // the result represents the size of the sent data
    // and close the connection now
    boost::system::error_code ec;
    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    socket.close();
}



void runClient3()
{
    using boost::asio::ip::tcp;
    boost::asio::io_context io_context;

    // we need a socket and a resolver
    tcp::socket socket(io_context);
    tcp::resolver resolver(io_context);

    // now we can use connect(..)
    boost::asio::connect(socket, resolver.resolve("localhost", "8080"));
    int my_port = 8081;
    // and use write(..) to send some data which is here just a string
    std::string data{"CALL Fari " + to_string(my_port)};
    auto result = boost::asio::write(socket, boost::asio::buffer(data));
    // auto result = boost::asio::write(socket, boost::asio::buffer(data));
    boost::asio::io_context io_context2;
    boost::asio::ip::tcp::acceptor acceptor(io_context2,boost::asio::ip::tcp::endpoint(tcp::v4(),my_port));
    boost::asio::ip::tcp::socket socket2(io_context);
    acceptor.accept(socket2);
    boost::asio::streambuf buffer;
    boost::system::error_code err_code;
    // here I need to read only first 16 bytes
    boost::asio::read_until(socket2, buffer,'\n',err_code);
    std::string message((std::istreambuf_iterator<char>(&buffer)), std::istreambuf_iterator<char>());
    // std::cout << data << std::endl;
    qDebug() << "response: " << message;
    // std::string resp((std::istreambuf_iterator<char>(&buff)), std::istreambuf_iterator<char>());
    // std::cout << resp << std::endl;
    // the result represents the size of the sent data
    // and close the connection now
    boost::system::error_code ec;
    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    socket.close();
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QThread serverThread;
    QThread clientThread;
    QThread clientThread2;
    // AudioRecorder ar;
    // ar.record();
    QObject::connect(&serverThread, &QThread::started, [](){
        runServer2();
    });
    QObject::connect(&clientThread, &QThread::started, [](){
        runClient2();
    });
    QObject::connect(&clientThread2, &QThread::started, [](){
        runClient3();
    });

    serverThread.start();
    clientThread.start();
    QThread::msleep(1000);
    clientThread2.start();

    MainWindow w;
    w.show();
    return a.exec();
}
