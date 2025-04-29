#include <iostream>
#include "chat.h"
#include "sha1.h"
using namespace std;

int main()
{
    HashTable chat;

    chat.reg((char*)"slava", (char*)"qwerty12345", 6); // *
    chat.reg((char*)"admin", (char*)"qwersty12314345", 20); // *
    chat.reg((char*)"popols", (char*)"qwerty1234s5", 3); // *
    chat.reg((char*)"root", (char*)"qwerty1fasf234s5", 5); // *
    chat.reg((char*)"boom", (char*)"gbfb213sva", 7); // *
    chat.reg((char*)"tendence", (char*)"bhtnrtyrbrb", 355); // *
    chat.reg((char*)"loordoom", (char*)"qwerty1234s5sa21", 16); // *
    chat.reg((char*)"chokotaco", (char*)"qww21f", 15); //*
    chat.reg((char*)"bambobi", (char*)"qwerty998855", 3);
    chat.printStats();
    chat.show();
    chat.del((char*)"admin");
    cout << endl;
    chat.login((char*)"admin", (char*)"qwerty12345", 20);
    chat.show();

    

    


    return 0;
}
