#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QDateTime>
#include <iostream>

class Logger {
public:
    enum Level {
        INFO,
        DEBUG,
        WARNING,
        ERROR,
        CRYPTO
    };

    static void log(Level level, const QString &message, const char* file = "", int line = 0) {
        QString timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
        QString levelStr;
        QString colorCode;

        switch (level) {
            case INFO:    levelStr = "[INFO]   "; colorCode = "\033[32m"; break; // Green
            case DEBUG:   levelStr = "[DEBUG]  "; colorCode = "\033[36m"; break; // Blue
            case WARNING: levelStr = "[WARNING]"; colorCode = "\033[33m"; break; // Yellow
            case ERROR:   levelStr = "[ERROR]  "; colorCode = "\033[31m"; break; // Red
            case CRYPTO:  levelStr = "[CRYPTO] "; colorCode = "\033[35m"; break; // Purple
        }

        // Form: [Time] [Level] [File:Line] Message
        std::cout << colorCode.toStdString()
                  << "[" << timeStr.toStdString() << "] "
                  << levelStr.toStdString()
                  << " " << message.toStdString();

        if (file && strlen(file) > 0) {
            std::cout << " (" << file << ":" << line << ")";
        }

        std::cout << "\033[0m" << std::endl; // Reset color
    }
};

#define LOG_INFO(msg)    Logger::log(Logger::INFO, msg, __FILE__, __LINE__)
#define LOG_DEBUG(msg)   Logger::log(Logger::DEBUG, msg, __FILE__, __LINE__)
#define LOG_WARNING(msg) Logger::log(Logger::WARNING, msg, __FILE__, __LINE__)
#define LOG_ERROR(msg)   Logger::log(Logger::ERROR, msg, __FILE__, __LINE__)
#define LOG_CRYPTO(msg)  Logger::log(Logger::CRYPTO, msg, __FILE__, __LINE__)

#endif // LOGGER_H
