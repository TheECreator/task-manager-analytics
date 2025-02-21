#include "database.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

Database::Database(const std::string &dbName) {
    if (sqlite3_open(dbName.c_str(), &db)) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Database opened successfully!" << std::endl;
        createTable();
    }
}

Database::~Database() {
    sqlite3_close(db);
}

void Database::createTable() {
    std::string query = "CREATE TABLE IF NOT EXISTS tasks ("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "title TEXT NOT NULL, "
                        "description TEXT, "
                        "status TEXT CHECK(status IN ('pending', 'completed', 'failed')) DEFAULT 'pending', "
                        "priority TEXT, "
                        "due_date TEXT, "
                        "completed_at TEXT, "
                        "deadline TEXT, "
                        "created_at TEXT DEFAULT CURRENT_TIMESTAMP);";
    executeQuery(query);
}

void Database::executeQuery(const std::string &query) {
    char *errMsg;
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL Error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Query executed successfully: " << query << std::endl;
    }
}

void Database::addTask(const std::string &title, const std::string &description, const std::string &priority,
                       const std::string &due_date, const std::string &completed_at, const std::string &deadline) {
    // Set status to 'completed' if completed_at is not empty, otherwise 'pending'
    std::string status = completed_at.empty() ? "pending" : "completed";

    // Insert the task into the database
    std::string query = "INSERT INTO tasks (title, description, priority, status, due_date, completed_at, deadline, created_at) VALUES ('" +
                        title + "', '" + description + "', '" + priority + "', '" + status + "', '" + due_date + "', '" + completed_at + "', '" + deadline + "', CURRENT_TIMESTAMP);";
    executeQuery(query);
    std::cout << "Task added successfully with status: " << status << std::endl;
}

std::vector<Task> Database::getTasks() {
    std::cout << "Executing SELECT query..." << std::endl;  // Debugging line
    std::vector<Task> tasks;
    std::string query = "SELECT * FROM tasks;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Task task;
            task.id = sqlite3_column_int(stmt, 0);
            task.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            task.description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            task.status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            task.priority = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            task.due_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            task.completed_at = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
            task.deadline = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
            task.created_at = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
            tasks.push_back(task);
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
    }
    return tasks;
}

void Database::updateTaskStatus(int id, const std::string &newStatus) {
    std::string query = "UPDATE tasks SET status = '" + newStatus + "' WHERE id = " + std::to_string(id) + ";";
    executeQuery(query);
    std::cout << "Task updated successfully.\n";
}

void Database::deleteTask(int id) {
    std::string query = "DELETE FROM tasks WHERE id = " + std::to_string(id) + ";";
    executeQuery(query);
    std::cout << "Task deleted successfully.\n";
}

void Database::checkPastDeadlines() {
    std::vector<Task> tasks = getTasks();
    for (auto &task : tasks) {
        if (task.status == "pending" && isPastDeadline(task.deadline)) {
            updateTaskStatus(task.id, "failed");
            std::cout << "Task ID " << task.id << " marked as failed due to missed deadline.\n";
        }
    }
}

bool Database::isPastDeadline(const std::string &deadline) {
    std::tm tm = {};
    std::istringstream ss(deadline);
    
    // Manually parse the date string (format YYYY-MM-DD)
    ss >> std::get_time(&tm, "%Y-%m-%d");

    if (ss.fail()) {
        std::cerr << "Error parsing deadline: " << deadline << std::endl;
        return false; // Return false if parsing failed
    }

    // Convert tm to time_t (system time)
    std::time_t deadline_time = std::mktime(&tm);

    // Get current time (system time)
    std::time_t now = std::time(nullptr);

    // Compare current time and deadline time
    return deadline_time < now;  // If deadline is in the past, return true
}

