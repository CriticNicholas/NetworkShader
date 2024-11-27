#include <asio.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <optional>

class Socket
{
public:
    Socket(const std::string_view Ip, int Port)
        : EndPoint(asio::ip::make_address(Ip.data()), Port),
        AsioSocket(IO_Context) {}

    void Connect() {
        if (AsioSocket.is_open())
        {
            return;
        }

        AsioSocket.async_connect(EndPoint, [this](asio::error_code ec) {
            if (ec) {
                std::cerr << "Connect error: " << ec.message() << std::endl;
            }
            else {
                std::cout << "Connect try" << std::endl;
            }
            });
    }

    void Host() {
        if (AsioSocket.is_open())
        {
            return;
        }
        
        Acceptor.emplace(IO_Context, EndPoint);

        Acceptor->listen();
        Acceptor->async_accept(AsioSocket, [this](asio::error_code ec) {
            if (ec) {
                std::cerr << "Accept error: " << ec.message() << std::endl;
            }
            else {
                std::cout << "Client connected!" << std::endl;
            }

            QuietReading();
        });
    }

    void Send(const std::string message)
    {

        asio::async_write(AsioSocket, asio::buffer(message), [this](asio::error_code ec, size_t bytes_transferred) {
            if (ec)
            {
                std::cerr << "Send error: " << ec.message() << std::endl;
            }
            });
    }

    std::string Read()
    {
        std::unique_lock<std::mutex> Lock(Mutex);
        ConditionVar.wait(Lock, [this]() { return !ReceivedMessages.empty(); });

        std::string Message = ReceivedMessages.front();

        ReceivedMessages.pop();

        return Message;
    }

    void Run()
    {
        std::thread([this]() { IO_Context.run(); }).detach();
    }

    void Stop()
    {
        AsioSocket.close();

        IO_Context.stop();
    }

    ~Socket()
    {
        try
        {
            Stop();
        }
        catch (const std::exception& e)
        {
            std::cerr << "Destructor error: " << e.what() << std::endl;
        }
    }

private:
    void QuietReading()
    {
        asio::async_read_until(AsioSocket, StreamBuffer, '\0', [this](asio::error_code ErrorCode, size_t BytesTransferred) {
            if (ErrorCode) {
                if (ErrorCode.value() == 10054)
                {
                    AsioSocket.close();

                    Host();
                }
                else if (ErrorCode.value() == 1236)
                {
                    AsioSocket.close();

                    Stop();
                }
                std::cerr << "Read Error: " << ErrorCode.value() << ' ' << ErrorCode.message() << std::endl;
                return;
            }

            std::istream InputStream(&StreamBuffer);

            std::string Message;

            std::getline(InputStream, Message, '\0');

            std::lock_guard<std::mutex> Lock(Mutex);

            ReceivedMessages.push(Message);

            ConditionVar.notify_one();

            QuietReading();
            }
        );
    }
    asio::streambuf StreamBuffer;

    std::mutex Mutex;

    std::condition_variable ConditionVar;

    std::queue<std::string> ReceivedMessages;

    std::optional<asio::ip::tcp::acceptor> Acceptor;

protected:
    asio::io_context IO_Context;

    asio::ip::tcp::socket AsioSocket;

    asio::ip::tcp::endpoint EndPoint;


};
