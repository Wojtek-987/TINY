#include <iostream>

using std::string;
using std::cout;
using std::cin;
using std::endl;


int main() {
    bool is_running = true;
    string user_command;

    while(is_running) {
        cout << "\033[1m\033[34m" << "tiny" << "\033[0m" << " < ";
        cin >> user_command;
        cout << "\033[1m\033[34m" << "tiny" << "\033[0m" << " > " << "\033[35m" << "undefined: " << user_command << "\033[0m" << endl;
        if(user_command == "exit")
            is_running = false;
    }

    return 0;
}