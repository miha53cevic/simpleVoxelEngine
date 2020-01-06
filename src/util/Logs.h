#pragma once

#include <fstream>
#include <string>
#include <ctime>

class Logs
{
public:
    Logs()
    {
        // Clear any previous log data
        std::ofstream writer("log.txt", std::ios::trunc);
        writer << "Start of log\n\n\n";
        writer.close();
    }

    static Logs& get()
    {
        static Logs log;
        return log;
    }

    void Write(std::string string)
    {
        std::ofstream writer("log.txt", std::ios::app);
        writer << string << "\n";
        writer.close();
    }

private:
};
