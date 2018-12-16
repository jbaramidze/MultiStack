#include <bits/stdc++.h>
#include "MyStackContainer.h"
using namespace std;

#define DEBUG

int main() {
    cout << "Size of whole array and # of stacks inside:\n";
    int size, splits;
    cin >> size >> splits;
    MyStackContainer *stacks = new MyStackContainer(size, splits);

    cout << "Operations:\n";
    cout << "Push to stack #i value v: push i v\n";
    cout << "Pop from stack #i: pop i\n";
    cout << "Top of stack #i: top i\n";
    cout << "Dump everything: dump\n";

    int err = 0;

    while (true) {
        string command;
        cin >> command;

        if (command == "push") {
            int i, v;
            cin >> i >> v;

            err = 0;
            stacks -> push(i, v, err);

            #ifdef DEBUG
            cout << "Push with i: " << i << ", v: " << v << " error " << err << endl;
            #endif
        } 
        else if (command == "pop") {
            int i;
            cin >> i;

            err = 0;
            int ret = stacks -> pop(i, err);

            #ifdef DEBUG
            cout << "Pop with i: " << i << " returned " << ret << " error " << err << endl;
            #endif
        }
        else if (command == "top") {
            int i;
            cin >> i;

            int ret = stacks -> top(i, err);

            #ifdef DEBUG
            cout << "Top with i: " << i << " returned " << ret << " error " << err << endl;
            #endif
        }
        else if (command.find("dump") != string::npos) {
            stacks -> dump();
        } 
        else {
            cout << "Invalid command\n";
        }

    }



    return 0;
}
