# Circular Logging System

This is a configurable circular logging system developed using **C++** and the **Qt Framework**. It is designed to be easily integrated into other projects and supports time-based log file creation and automatic rotation.

---

## 🔧 Features

- Supports logging by:
  - Daily
  - Hourly
  - Minutely
  - Secondly
- Log rotation (oldest logs are automatically deleted)
- Adjustable logging frequency (`log_frequency`)
- Logs saved in UTC format
- Configuration is read from a `config.json` file

---

## 📁 Project Structure

```
├── main.cpp
├── Logger.h
├── Logger.cpp
├── config.json      ← User must adjust this path manually
```

---

## ⚙️ Configuration File (`config.json`)

This file defines the logging behavior and must be present at a known path.

```json
{
  "log_type": "daily",
  "log_frequency": 1,
  "max_log_count": 30,
  "log_directory": "C:/Users/furkan/Desktop/logs"
}
```

### ❗ Important

> ⚠️ The path to `config.json` is currently **hardcoded** in `main.cpp`:
>
> ```cpp
> logger = new Logger("C:/Users/furkan/Desktop/CircularLogging_Piton/CircularLogging/config.json");
> ```
>
> You must **update this path** according to your own system.

---

## 🚀 How It Works

1. The application reads settings from `config.json`
2. A timer waits until 18:00 every day
3. At 18:00:
   - A new log file is created (if frequency allows)
   - Log content includes timestamp and task count
   - Old logs are deleted if count exceeds `max_log_count`

---

## 🛠️ Technologies Used

- **C++17**
- **Qt 6.9**
- QtCore modules: `QDateTime`, `QTimer`, `QFile`, `QJsonDocument`

---

## ✍️ Author

**Furkan Kara**  
> Electrical and Electronics Engineering Student  
> Eskişehir Osmangazi University  
> [LinkedIn] https://www.linkedin.com/in/furkankara32 