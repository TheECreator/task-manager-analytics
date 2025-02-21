#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>
#include <vector>

struct Task {
    int id;
    std::string title;
    std::string description;
    std::string status;
    std::string priority;
    std::string due_date;
    std::string completed_at;
    std::string deadline;
    std::string created_at;
};

class Database {
public:
    Database(const std::string &dbName);
    ~Database();
    
    void createTable();
    void addTask(const std::string &title, const std::string &description, const std::string &priority, 
                 const std::string &due_date, const std::string &completed_at, const std::string &deadline);
    std::vector<Task> getTasks();
    void updateTaskStatus(int id, const std::string &newStatus);
    void deleteTask(int id);
    
    void checkPastDeadlines();  // New method to check and update overdue tasks
    bool isPastDeadline(const std::string &deadline);  // Helper function to compare dates

private:
    sqlite3 *db;
    void executeQuery(const std::string &query);
};

#endif
