#include <iostream>
#include "database.h"

void showMenu() {
    std::cout << "\nTask Manager Menu:\n";
    std::cout << "1. Add Task\n";
    std::cout << "2. View Tasks\n";
    std::cout << "3. Mark Task as Completed\n";
    std::cout << "4. Delete Task\n";
    std::cout << "5. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    Database db("database/tasks.db");

    while (true) {
        showMenu();
        int choice;
        std::cin >> choice;
        std::cin.ignore(); // Clear input buffer

        if (choice == 1) {
            std::string title, description, priority, due_date, completed_at, deadline;
            std::cout << "Enter task title: ";
            std::getline(std::cin, title);
            std::cout << "Enter task description: ";
            std::getline(std::cin, description);
            std::cout << "Enter task priority: ";
            std::getline(std::cin, priority);
            std::cout << "Enter due date (YYYY-MM-DD): ";
            std::getline(std::cin, due_date);
            std::cout << "Enter completed date (YYYY-MM-DD) or leave blank: ";
            std::getline(std::cin, completed_at);
            std::cout << "Enter deadline (YYYY-MM-DD): ";
            std::getline(std::cin, deadline);

            db.addTask(title, description, priority, due_date, completed_at, deadline);
        } 
        else if (choice == 2) {
            std::vector<Task> tasks = db.getTasks();
            if (tasks.empty()) {
                std::cout << "No tasks found or there was an error fetching tasks.\n";
            } else {
                std::cout << "\nTask List:\n";
                for (const auto &task : tasks) {
                    std::cout << "ID: " << task.id << " | " 
                              << task.title << " | "
                              << task.status << " | "
                              << task.priority << " | "
                              << task.due_date << " | "
                              << task.completed_at << " | "
                              << task.deadline << "\n";
                }
            }
        }
        
        else if (choice == 3) {
            int id;
            std::cout << "Enter task ID to mark as completed: ";
            std::cin >> id;
            db.updateTaskStatus(id, "completed");
        } 
        else if (choice == 4) {
            int id;
            std::cout << "Enter task ID to delete: ";
            std::cin >> id;
            db.deleteTask(id);
        } 
        else if (choice == 5) {
            std::cout << "Exiting Task Manager.\n";
            break;
        } 
        else {
            std::cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
