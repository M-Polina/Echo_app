#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

enum {
    max_length = 1024
};

class Echo_client {
public:
    Echo_client(boost::asio::io_service &io_service, std::string host, std::string port)
//            : io_service_(io_service), resolver_(io_service), query_(tcp::v4(), host, port),
    : io_service_(io_service), resolver_(io_service), query_(boost::asio::ip::address::from_string(host), std::atoi(port.data())),
              socket_(io_service){
//        iterator_ = resolver_.resolve(query_);
        socket_connect();
        start();
    }

    ~Echo_client() {
        socket_.close();
    }

    void socket_connect(){
//        socket_.connect(*iterator_);
        socket_.connect(query_);
    }

    void start(){
        get_request();
    }

    void get_request(){
        char request[max_length];
        std::cout << "Enter message: ";
        std::cin.getline(request, max_length);
        size_t request_length = strlen(request);
        handle_write(request, request_length);
    }

    void handle_write(char* request, size_t request_length){
        boost::asio::write(socket_, boost::asio::buffer(request, request_length));
        handle_get_reply(request_length);
    }

    void handle_get_reply(size_t request_length)
    {
        char reply[max_length];
        size_t reply_length = boost::asio::read(socket_,
                                                boost::asio::buffer(reply, request_length));
        std::cout << "Reply: ";
        std::cout.write(reply, reply_length);
        std::cout << "\n";
        get_request();
    }

private:
    boost::asio::io_service &io_service_;
    tcp::resolver resolver_;
    tcp::endpoint query_;
//    tcp::resolver::query query_;
    tcp::resolver::iterator iterator_;
    tcp::socket socket_;
};


int main(int argc, char *argv[]) {
    try {
        boost::asio::io_service io_service;

        Echo_client client(io_service, "localhost", "80");

    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
