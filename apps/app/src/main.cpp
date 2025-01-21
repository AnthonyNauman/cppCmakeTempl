#include <iostream>
#include <string>
#include <someClass.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>


//@ \brief main
int main()
{
    SomeClass c;
    c.sum(4,5);
    auto logger = spdlog::daily_logger_mt("bas", "logs/1.log");
    logger->info("Hello, logger!");
    spdlog::flush_every(std::chrono::seconds(3));
    std::cout << "Hello, world!" << std::endl;
    auto ch = std::getchar();
}