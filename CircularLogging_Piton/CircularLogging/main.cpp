/*
Furkan KARA 16.06.2025

PSEUDOCODE: Daily Logging System

1. Load logging configuration from "config.json"
2. Create a logger instance using config values
3. Schedule a timer to wait until 18:00
4. When timer triggers:
    a. Write today's task count to a daily log file
    b. Delete oldest logs if log file count exceeds max_log_count
    c. Reset the timer for the next day's 18:00
5. Application runs continuously in the background
*/

#include <QCoreApplication>
#include <QTimer>
#include <QDateTime>
#include <QTime>
#include <QDebug>
#include "Logger.h"

Logger* logger = nullptr;

// This function is called every day at 18:00
void performDailyLog() {
    logger->logTaskCount(12);
    qInfo() << "18:00 log entry has been written.";
}

// This calculates how many milliseconds are left until next 18:00
qint64 millisecondsUntilNext18() {
    QDateTime now = QDateTime::currentDateTime();
    QDateTime target = now;
    target.setTime(QTime(18, 0, 0));
    if (now.time() >= QTime(18, 0, 0)) {
        target = target.addDays(1);
    }
    return now.msecsTo(target);
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    // create logger and read settings from config file
    logger = new Logger("C:/Users/furkan/Desktop/CircularLogging_Piton/CircularLogging/config.json");

    /* You can test.
    logger->logTaskCount(12);
    */

    QTimer* timer = new QTimer(&app);
    QObject::connect(timer, &QTimer::timeout, [timer]() {
        performDailyLog();
        timer->stop();
        timer->start(millisecondsUntilNext18());
    });

    timer->start(millisecondsUntilNext18());

    return app.exec();
}
