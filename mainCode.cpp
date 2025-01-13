#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <filesystem>


// Namespace declaration
using namespace std;
namespace fs = std::filesystem;

// Global Variables
vector<string> history;

// Pre-decleration of builtin function
int lsh_cd(vector<char *> args);
int lsh_help(vector<char *> args);
int lsh_exit(vector<char *> args);
int lsh_history(vector<char *> args);

vector<char *> builtin_str = {
    "cd",
    "help",
    "exit",
    "history"};

vector<int (*)(vector<char *>)> builtin_func =
    {
        lsh_cd,
        lsh_help,
        lsh_exit,
        lsh_history
    };

// -----------------------------builtin functions
int lsh_num_builtins()
{
    return builtin_str.size();
}

int lsh_cd(vector<char *> args)
{
    if (args[1] == nullptr)
    {
        cerr << "lsh: expected argument to \"cd\"" << endl;
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("lsh");
        }
    }
    return 1;
}

int lsh_help(vector<char *> args)
{
    int i;
    cout << "Type program names and arguments, and hit enter" << endl;
    cout << "The following are built in :" << endl;

    for (i = 0; i < lsh_num_builtins(); i++)
    {
        cout << builtin_str[i] << endl;
    }

    cout << "Use the man command for information on other programs." << endl;
    return 1;
}

int lsh_exit(vector<char *> args)
{
    return 0;
}

int lsh_history(vector<char *> args)

{
    int n = history.size();
    if(n == 0)
        cout<<"No Previous command Found"<<endl;
    else if(n <= 5)
    {
        for(auto s : history)
            cout<<s<<endl;
    }
    else
    {
        for(auto it = history.end() - 5; it != history.end(); it++)
            cout<<*it<<endl;
    }
    return 1;
}
// ----------------------------miscellaneous function
string get_pwd(void)
{
    string temp = fs::current_path();
    return temp;
}

// ----------------------------major functions 
vector<char *> lsh_split_line(string line)
{
    char delimeter = ' ';

    istringstream ss(line);

    vector<string> tokens;
    string token;

    while (getline(ss, token, delimeter))
    {
        tokens.push_back(token);
    }
    vector<char *> c_args;
    for (auto &arg : tokens)
    {
        c_args.push_back(const_cast<char *>(arg.c_str()));
    }
    c_args.push_back(nullptr);
    return c_args;
}

int lsh_launch(vector<char *> args)
{
    pid_t pid, wpid;
    int status;
    pid = fork();
    if (pid == 0)
    {
        if (execvp(args[0], args.data()) == -1)
        {
            perror("execvp failed");
            return 1;
        }
    }
    else if (pid < 0)
    {
        perror("lsh");
    }
    else
    {
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
            if (wpid == -1)
            {
                perror("waitpid failed");
                return 1;
            }
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));

    }
    return 1;
}

int lsh_execute(vector<char *> args)
{
    int i;

    if (args[0] == NULL)
    {
        return 1;
    }

    for (int i = 0; i < lsh_num_builtins(); i++)
    {
        if (strcmp(args[0], builtin_str[i])==0)
            return (builtin_func[i])(args);
    }

    return lsh_launch(args);
}

void lsh_loop()
{
    string line;
    vector<char *> args;
    int status;
    do
    {
        cout<<get_pwd()<<"$ ";
        getline(cin, line);
        history.push_back(line);
        args = lsh_split_line(line);
        status = lsh_execute(args);
    } while (status);
}

// main function
int main()
{
    cout << "Welcome to basic Shell" << endl;
    lsh_loop();
    return 0;
}
