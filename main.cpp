#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Echo_session {
public:
    Echo_session(boost::asio::io_service &io_service)
            : socket_(io_service) {
    }

    tcp::socket &socket() {
        return socket_;
    }

    void start() {
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                boost::bind(&Echo_session::handle_write, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
    }

    void handle_write(const boost::system::error_code &error,
                      size_t bytes_transferred) {
        if (!error) {
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(data_, bytes_transferred),
                                     boost::bind(&Echo_session::handle_read, this,
                                                 boost::asio::placeholders::error));
        } else {
            std::cout << "Session stopped and deleted: " << this << std::endl;
            delete this;
        }
    }

    void handle_read(const boost::system::error_code &error) {
        if (!error) {
            socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                    boost::bind(&Echo_session::handle_write, this,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
        } else {
            std::cout << "Error handle_write" << std::endl;
            delete this;
        }
    }

private:
    tcp::socket socket_;
    enum {
        max_length = 1024
    };
    char data_[max_length];
};

class Echo_server {
public:
    Echo_server(boost::asio::io_service &io_service, short port)
            : io_service_(io_service),
              acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {
        Echo_session *new_session = new Echo_session(io_service_);
        std::cout << "New Session was created: " << new_session << std::endl;

        acceptor_.async_accept(new_session->socket(),
                               boost::bind(&Echo_server::handle_accept, this, new_session,
                                           boost::asio::placeholders::error));
    }

    void handle_accept(Echo_session *new_session,
                       const boost::system::error_code &error) {
        if (!error) {
            new_session->start();
            new_session = new Echo_session(io_service_);
            std::cout << "New Session was created: " << new_session << std::endl;
            acceptor_.async_accept(new_session->socket(),
                                   boost::bind(&Echo_server::handle_accept, this, new_session,
                                               boost::asio::placeholders::error));
        } else {
            std::cout << "Error handle_accept" << std::endl;
            delete new_session;
        }
    }

private:
    boost::asio::io_service &io_service_;
    tcp::acceptor acceptor_;
};

int main(int argc, char *argv[]) {
    try {
        boost::asio::io_service io_service;
        Echo_server server(io_service, std::atoi("80"));
        io_service.run();
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}











