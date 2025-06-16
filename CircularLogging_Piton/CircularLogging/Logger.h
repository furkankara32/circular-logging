#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QDateTime>

// Logger class for creating and managing log files
class Logger {
public:
    Logger(const QString& configPath);
    void logTaskCount(int completedTasks);

private:
    QString logType;
    int maxLogCount;
    QString logDirectory;
    int logFrequency;
    QDateTime lastLogTime;

    void loadConfig(const QString& configPath);
    QString getLogFileName();
    void rotateLogs();
    bool shouldLogNow();
};

#endif // LOGGER_H
