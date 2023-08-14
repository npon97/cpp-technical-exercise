#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <mutex>
#include <ctime>
#include <chrono>
#include <iostream>
#include <sys/time.h>
#include <ctime>

enum LogPriority
{
    TracePriority,
    DebugPriority,
    InfoPriority,
    WarnPriority,
    ErrorPriority,
    CriticalPriority
};

class Logger
{
private:
    LogPriority priority = InfoPriority;
    std::mutex logMutex;
    const char *filepath = 0;
    FILE *file = 0;

    // Singleton
    Logger() {}
    Logger(const Logger &) = delete; // Won't be public but good for formality
    Logger &operator=(const Logger &) = delete;
    ~Logger()
    {
        freeFile();
    }

    static Logger &getInstance()
    {
        static Logger logger;
        return logger;
    }

    template <typename... Args>
    void log(const char *messagePriorityStr, LogPriority messagePriority, const char *message, Args... args)
    {
        if (priority <= messagePriority)
        {
            std::time_t currentTime = std::time(0);
            std::tm *timestamp = std::localtime(&currentTime);
            char buffer[80];
            strftime(buffer, 80, "%FT%T", timestamp);

            std::lock_guard<std::mutex> lock(logMutex);
            printf("[%s]\t", buffer);
            printf(messagePriorityStr);
            printf("\t");
            printf(message, args...);
            printf("\n");

            if (file)
            {
                fprintf(file, "[%s]\t", buffer);
                fprintf(file, messagePriorityStr);
                fprintf(file, "\t");
                fprintf(file, message, args...);
                fprintf(file, "\n");
            }
        }
    }

    void genericEnableFileOutput()
    {
        if (file != 0)
        {
            fclose(file);
        }

        file = fopen(filepath, "a");

        if (file == 0)
        {
            printf("Logger: Failed to open file at %s", filepath);
        }
    }

    void freeFile()
    {
        if (file != nullptr)
        {
            fclose(file);
            file = 0;
        }
    }

public:
    static void SetPriority(LogPriority newPriority)
    {
        Logger &logger_instance = getInstance();
        logger_instance.priority = newPriority;
    }

    static void EnableFileOutput()
    {
        Logger &logger_instance = getInstance();
        logger_instance.filepath = "log.txt"; // TODO: get proper path

        logger_instance.genericEnableFileOutput();
    }

    static void EnableFileOutput(const char *newFilePath)
    {
        Logger &logger_instance = getInstance();
        logger_instance.filepath = newFilePath;

        logger_instance.genericEnableFileOutput();
    }

    template <typename... Args>
    static void Trace(const char *message, Args... args)
    {
        getInstance().log("[Trace]", TracePriority, message, args...);
    }

    template <typename... Args>
    static void Debug(const char *message, Args... args)
    {
        getInstance().log("[Debug]", DebugPriority, message, args...);
    }

    template <typename... Args>
    static void Info(const char *message, Args... args)
    {
        getInstance().log("[Info]", InfoPriority, message, args...);
    }

    template <typename... Args>
    static void Warn(const char *message, Args... args)
    {
        getInstance().log("[Warn]", WarnPriority, message, args...);
    }

    template <typename... Args>
    static void Error(const char *message, Args... args)
    {

        getInstance().log("[Error]", ErrorPriority, message, args...);
    }

    template <typename... Args>
    static void Critical(const char *message, Args... args)
    {
        getInstance().log("[Critical]", CriticalPriority, message, args...);
    }
};

#endif /* LOGGER_H */