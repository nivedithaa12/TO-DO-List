#include <iostream> 
#include <vector> 
#include <string> 
 
using namespace std; 
 
class Task { 
public: 
    Task(string name, string description, bool completed = false) { 
        this->name = name; 
        this->description = description; 
        this->completed = completed; 
    } 
 
    string getName() { 
        return name; 
    } 
 
    string getDescription() { 
        return description; 
    } 
 
    bool isCompleted() { 
        return completed; 
    } 
 
    void setCompleted(bool completed) { 
        this->completed = completed; 
    } 
 
private: 
  string name; 
12 
 
     
string description; 
    bool completed; 
}; 
 
class ToDoList { 
public: 
    ToDoList() { 
        tasks = vector<Task>(); 
    } 
 
    void addTask(Task task) { 
        tasks.push_back(task); 
    } 
 
    void removeTask(int index) { 
        tasks.erase(tasks.begin() + index); 
    } 
 
    void printTasks() { 
        for (int i = 0; i < tasks.size(); i++) { 
            cout << i << ". " << tasks[i].getName() << " - " << tasks[i].getDescription() << " - "; 
            if (tasks[i].isCompleted()) { 
                cout << "Completed"; 
            } else { 
                cout << "Not completed"; 
            } 
            cout << endl; 
        } 
    } 
 
    void completeTask(int index) { 
        tasks[index].setCompleted(true); 
    } 
 
13 
 
 
private: 
    vector<Task> tasks; 
}; 
 
int main() { 
    ToDoList toDoList = ToDoList(); 
    while (true) { 
        cout << "What would you like to do?" << endl; 
        cout << "1. Add a task" << endl; 
        cout << "2. Remove a task" << endl; 
        cout << "3. Print tasks" << endl; 
        cout << "4. Complete a task" << endl; 
        cout << "5. Exit" << endl; 
 
        int choice; 
        cin >> choice; 
 
        switch (choice) { 
            case 1: { 
                string name, description; 
                cout << "Enter task name: "; 
                cin.ignore(); 
                getline(cin, name); 
                cout << "Enter task description: "; 
                getline(cin, description); 
                toDoList.addTask(Task(name, description)); 
                break; 
            } 
            case 2: { 
                int index; 
                  
                ocout << "Enter task index: "; 
                cin >> index; 
                 
14 
 
            
              toDoList.removeTask(index); 
               
               break; 
            } 
            case 3: { 
                toDoList.printTasks(); 
                break; 
            } 
            case 4: { 
                int index; 
                cout << "Enter task index: "; 
                cin >> index; 
                toDoList.completeTask(index); 
                break; 
            } 
            case 5: { 
                return 0; 
            } 
            default: { 
                cout << "Invalid choice" << endl; 
                break; 
            } 
        } 
    } 
}    
