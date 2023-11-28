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

//        request.append("\n");
//        size_t request_length = std::strlen(request);
        handle_write(request);
    }

    void handle_write(std::string request){
        boost::asio::write(socket_, boost::asio::buffer(request, request.size()));
        handle_get_reply( request.size());
    }

    void handle_get_reply(size_t request_length)
    {
//        std::string str = "123456";
//        str.erase(str.size()-4);
//        std::cout << str;
        boost::asio::streambuf reply;
//        har reply[max_length];
//
        boost::asio::read_until(socket_, reply, "\r\n\r\n");
        std::istream reply_istream(&reply);
        std::string reply_str;
        std::getline(reply_istream, reply_str);
        std::cout << reply_str.size() << std::endl;
        reply_str.erase(reply_str.size() - 1);
        std::cout << "Reply: " << reply_str << std::endl;
        get_request();

//                                "\r\n\r\n",
//        write_reply(reply);
//                               std::bind(&Echo_client::write_reply, this, std::placeholders::_1, std::placeholders::_2));
////                                [self](boost::system::error_code ec, std::size_t length)
////                                     {
////                                         if (!ec)
////                                         {
////                                             std::cout << "Reply: ";
////                                             std::cout.write(reply, request_length);
////                                             std::cout << "\n";
////                                             self->get_request();
////                                         }
////                                     });
//        size_t reply_length = boost::asio::read(socket_,
//                                                boost::asio::buffer(reply, request_length));
//        std::cout << "Reply: ";
//        std::cout.write(reply, reply_length);
//        std::cout << "\n";
//        get_request();
    }

//    void handle_get_reply(size_t request_length)
//    {
//        char reply[max_length];
//        size_t reply_length = boost::asio::read(socket_,
//                                                boost::asio::buffer(reply, request_length));
//
//
//        std::cout << "Reply: ";
//        std::cout.write(reply, reply_length);
//        std::cout << "HI";
//        std::cout << "\n";
//        get_request();
//    }

//    void write_reply(boost::system::error_code ec, std::size_t reply_len) {
//void write_reply(boost::asio::streambuf reply) {
//        if (ec) {
//            std::cout << "Disconnection!" << std::endl;
//        } else {
//            std::istream reply_istream(&reply);
//            std::string reply_str;
//            std::getline(reply_istream, reply_str);
//            reply_str.erase(reply_str.size() - 4);
//            std::cout << "Reply: " << reply_str << std::endl;
//
////            std::cout << "Reply: ";
////            std::cout.write(reply, reply_length);
////            std::cout << "\n";
//            get_request();
//        }
//    }

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