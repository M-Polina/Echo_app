#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Echo_client {
public:
    Echo_client(boost::asio::io_service &io_service, std::string host, std::string port)
            : io_service_(io_service), resolver_(io_service), query_(boost::asio::ip::address::from_string(host), std::atoi(port.data())),
              socket_(io_service){
        socket_connect();
        start();
    }

    ~Echo_client() {
        socket_.close();
    }

    void socket_connect(){
        socket_.connect(query_);
    }

    void start(){
        get_request();
    }

    void get_request(){
        std::cout << "Enter message: ";
        std::string request;

        std::getline(std::cin, request);
        request.append("\r\n\r\n");
        std::cout << request.size() << std::endl;

        handle_write(request);
    }

    void handle_write(std::string request){
        boost::asio::write(socket_, boost::asio::buffer(request, request.size()));
        handle_get_reply( request.size());
    }

    void handle_get_reply(size_t request_length)
    {
        boost::asio::streambuf reply;
        boost::asio::read_until(socket_, reply, "\r\n\r\n");
        std::istream reply_istream(&reply);
        std::string reply_str;
        std::getline(reply_istream, reply_str);
        std::cout << reply_str.size() << std::endl;
        reply_str.erase(reply_str.size() - 1);
        std::cout << "Reply: " << reply_str << std::endl;
        get_request();
    }

private:
    boost::asio::io_service &io_service_;
    tcp::resolver resolver_;
    tcp::endpoint query_;
    tcp::resolver::iterator iterator_;
    tcp::socket socket_;
};


int main(int argc, char *argv[]) {
    try {
        boost::asio::io_service io_service;

        Echo_client client(io_service, "127.0.0.1", "80");

    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}