#include <iostream>
#include <vector>
#include <string>
using namespace std;
// string lsh_read_line()
// {
    
// }
void lsh_loop()
{
    string line;
    vector<string> args;
    int status;

    do 
    {
    line = cin>>line;
    if(line == "quit")
    {
        std::cout<<"quiting..."<<endl;
        break;
    }
    // args = lsh_split_line(line);
    // status = lsh_execute(args);
    }while(status);
}
int main()
{
    lsh_loop();
    return 0;
}
