#include "Logger.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDateTime>
#include <QFileInfoList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

// Constructor: Load config and prepare log folder
Logger::Logger(const QString& configPath) {
    loadConfig(configPath);
    lastLogTime = QDateTime();
    if (logDirectory.isEmpty()) {
        qCritical() << "[ERROR] log_directory is empty! Please check config.json.";
        return;
    }
    QDir().mkpath(logDirectory);
}

// Read settings from config.json file
void Logger::loadConfig(const QString& configPath) {
    QFile file(configPath);
    if (!file.exists()) {
        qCritical() << "[ERROR] config.json not found:" << configPath;
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qCritical() << "[ERROR] Unable to open config.json:" << configPath;
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        qCritical() << "[ERROR] Invalid JSON format in config.json!";
        return;
    }

    QJsonObject obj = doc.object();

    logType = obj["log_type"].toString("daily");
    maxLogCount = obj["max_log_count"].toInt(30);
    logDirectory = obj["log_directory"].toString("./logs");
    logFrequency = obj["log_frequency"].toInt(1);

    qDebug() << "[INFO] config.json loaded successfully.";
    qDebug() << "log_type:" << logType;
    qDebug() << "max_log_count:" << maxLogCount;
    qDebug() << "log_directory:" << logDirectory;
}

// Return full log file name using today's date
QString Logger::getLogFileName() {
    QDateTime now = QDateTime::currentDateTimeUtc();
    QString format;

    if (logType == "hourly")
        format = "yyyy-MM-dd_HH";
    else if (logType == "minutely")
        format = "yyyy-MM-dd_HH-mm";
    else if (logType == "secondly")
        format = "yyyy-MM-dd_HH-mm-ss";
    else
        format = "yyyy-MM-dd";

    return logDirectory + "/" + now.toString(format) + ".log";
}

// Write task count into today's log file
void Logger::logTaskCount(int completedTasks) {
    if (!shouldLogNow()) {
        return;
    }

    rotateLogs();

    QString fileName = getLogFileName();
    QFile file(fileName);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << QDateTime::currentDateTimeUtc().toString(Qt::ISODate)
            << " - Number of tasks completed today: " << completedTasks << "\n";

        lastLogTime = QDateTime::currentDateTimeUtc();
    }
}

// Delete old log files if count exceeds maxLogCount
void Logger::rotateLogs() {
    QDir dir(logDirectory);
    QFileInfoList files = dir.entryInfoList(QStringList() << "*.log", QDir::Files, QDir::Time);
    while (files.size() >= maxLogCount) {
        QFile::remove(files.last().absoluteFilePath());
        files.removeLast();
    }
}

bool Logger::shouldLogNow() {
    if (!lastLogTime.isValid()) {
        return true;
    }

    QDateTime now = QDateTime::currentDateTimeUtc();

    if (logType == "daily")
        return lastLogTime.daysTo(now) >= logFrequency;

    if (logType == "hourly")
        return lastLogTime.secsTo(now) >= logFrequency * 3600;

    if (logType == "minutely")
        return lastLogTime.secsTo(now) >= logFrequency * 60;

    if (logType == "secondly")
        return lastLogTime.secsTo(now) >= logFrequency;

    return true;
}
