#include <iostream>
#include <cstdlib>
#include "compiler.hpp"
#include "utils.h"
#include "app.h"

using namespace std;

int main() {

#ifdef IS_WIN
    system("cls");
#else
    system("clear");
#endif

    cout << "AgileLB v1.0.1 " << '(' << COMPILE_TIME << ") [" << get_compiler() << ']' << endl;
    start();

}