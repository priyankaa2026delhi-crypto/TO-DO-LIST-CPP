#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
using namespace std;

struct Task
{
    string description;
    bool isDone;
};

const string FILENAME = "tasks.txt";

// Load tasks from file into the vector when program starts
void loadTasks(vector<Task> &tasks)
{
    ifstream file(FILENAME);
    if (!file)
        return; // file doesn't exist yet, that's fine

    string line;
    while (getline(file, line))
    {
        if (line.empty())
            continue;
        // Format in file: "0|Buy groceries" or "1|Finish report"
        char statusChar = line[0];
        string desc = line.substr(2); // skip "0|" or "1|"
        Task t;
        t.isDone = (statusChar == '1');
        t.description = desc;
        tasks.push_back(t);
    }
    file.close();
}

// Rewrite the entire file with current tasks (called after every change)
void saveTasks(const vector<Task> &tasks)
{
    ofstream file(FILENAME);
    for (const auto &t : tasks)
    {
        file << (t.isDone ? "1" : "0") << "|" << t.description << "\n";
    }
    file.close();
}

void printMenu()
{
    cout << "\n--- TO-DO LIST ---\n";
    cout << "1. Add Task\n";
    cout << "2. View Tasks\n";
    cout << "3. Mark Task as Done\n";
    cout << "4. Delete Task\n";
    cout << "5. Exit\n";
    cout << "Enter choice: ";
}

void viewTasks(const vector<Task> &tasks)
{
    if (tasks.empty())
    {
        cout << "No tasks yet.\n";
        return;
    }
    for (int i = 0; i < tasks.size(); i++)
    {
        cout << i + 1 << ". " << tasks[i].description;
        cout << (tasks[i].isDone ? " [Done]" : " [Pending]") << endl;
    }
}

int main()
{
    vector<Task> tasks;
    loadTasks(tasks); // load any previously saved tasks

    int choice;

    do
    {
        printMenu();
        cin >> choice;

        if (cin.fail())
        {
            // user typed something that's not a number — reset cin
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a valid number.\n";
            continue;
        }

        if (choice == 1)
        {
            Task t;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter task description: ";
            getline(cin, t.description);
            t.isDone = false;
            tasks.push_back(t);
            saveTasks(tasks);
            cout << "Task added!\n";
        }
        else if (choice == 2)
        {
            viewTasks(tasks);
        }
        else if (choice == 3)
        {
            if (tasks.empty())
            {
                cout << "No tasks to mark.\n";
            }
            else
            {
                viewTasks(tasks);
                int num;
                cout << "Enter task number to mark as done: ";
                cin >> num;
                if (num >= 1 && num <= (int)tasks.size())
                {
                    tasks[num - 1].isDone = true;
                    saveTasks(tasks);
                    cout << "Task marked as done!\n";
                }
                else
                {
                    cout << "Invalid task number.\n";
                }
            }
        }
        else if (choice == 4)
        {
            if (tasks.empty())
            {
                cout << "No tasks to delete.\n";
            }
            else
            {
                viewTasks(tasks);
                int num;
                cout << "Enter task number to delete: ";
                cin >> num;
                if (num >= 1 && num <= (int)tasks.size())
                {
                    tasks.erase(tasks.begin() + (num - 1));
                    saveTasks(tasks);
                    cout << "Task deleted!\n";
                }
                else
                {
                    cout << "Invalid task number.\n";
                }
            }
        }
        else if (choice != 5)
        {
            cout << "Invalid choice, try again.\n";
        }

    } while (choice != 5);

    cout << "Goodbye!\n";
    return 0;
}