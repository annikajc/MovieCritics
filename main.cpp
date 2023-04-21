#include <iostream>
#include <fstream>

using namespace std;

int main(){
    /* Declare variables here! */
    int menu1, menu2;
    string inputName; //stores the name of the actor or the movie to search for

    /* Displays menu and takes in input from the user */
    cout << "Welcome!\n\nMenu:\n1. Search for Actor\n2. Search for Movie\n";
    cin >> menu1;
    if (menu1 == 1)
        cout << "Which actor would you like to know about?\n";
    else if (menu1 == 2)
        cout << "Which movie would you like to know about?\n";
    getline(cin, inputName);
    cout << "1. Search using Map\n2. Search using B+ Tree\n";
    cin >> menu2;

    if (menu1 == 1 && menu2 == 1){
        /* Implement Actor Map */
    }

    else if (menu1 == 2 && menu2 == 1){
        /* Implement Movie Map */
    }

    else if (menu1 == 1 && menu2 == 2){
        /* Implement Actor B+ Tree */
    }

    else if (menu1 == 2 && menu2 == 2){
        /* Implement Movie B+ Tree */
    }
}