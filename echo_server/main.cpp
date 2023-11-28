#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session
        : public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket)
            : socket_(std::move(socket))
    {
    }

    void start()
    {
        do_read();
    }

private:
    void do_read()
    {
        boost::asio::async_read_until(socket_, response, "\r\n\r\n",
                                [self = shared_from_this()](boost::system::error_code ec, std::size_t length)
                                {
                                    if (!ec)
                                    {
                                        self->do_write(length);
                                    }
                                });
    }

    void do_write(std::size_t length)
    {
        boost::asio::async_write(socket_, response,
                                 [self = shared_from_this()](boost::system::error_code ec, std::size_t /*length*/)
                                 {
                                     if (!ec)
                                     {
                                         self->do_read();
                                     }
                                 });
    }

    tcp::socket socket_;
    boost::asio::streambuf response;
};

class server
{
public:
    server(boost::asio::io_service& io_service, short port)
            : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
              socket_(io_service)
    {
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(socket_,
                               [this](boost::system::error_code ec)
                               {
                                   if (!ec)
                                   {
                                       std::make_shared<session>(std::move(socket_))->start();
                                   }

                                   do_accept();
                               });
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
};

int main(int argc, char* argv[])
{
    try
    {
        boost::asio::io_service io_service;

        server s(io_service, std::atoi("80"));

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}


