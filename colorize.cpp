#include <iostream>
#include <string>
#include <map>
#include <bits/stdc++.h>
#include <vector>
#include <Windows.h>

using namespace std;

HANDLE console;
int conreset;
bool addSpase = false;

void reset() {
    if (!SetConsoleTextAttribute(console, conreset)) { // reset color
        cout << "============================================================" << endl;
        cout << "SetConsoleScreenBufferInfo, RESET: " << GetLastError() << endl;
        cout << "============================================================" << endl;
        exit(EXIT_FAILURE);
    }
}

void colorize(vector<pair<int, string>> parts) {
    int i = 0;
    SetConsoleOutputCP( 1251 ); // console encoding for Cyrillic
    for (const auto& pair : parts) {
        i++;
        if (! SetConsoleTextAttribute(console, pair.first)) { // set new colors
            cout << "============================================================" << endl;
            cout << "SetConsoleScreenBufferInfo, COLORIZE: " << GetLastError() << endl;
            cout << "============================================================" << endl;
        }
        if (i == parts.size()) {
            cout << pair.second << endl;
        } else {
            cout << pair.second;
            if (addSpase) {
                reset();
                cout << " ";
            }
        }
    }
}

string color(map<string, int> colors, int c) {
    for(auto pair : colors) {
        if(pair.second == c) {
            return pair.first;
        }
    }
    return ""; 
}

int main(int argc, char* argv[]) {

    map<string, int> colors = {
        {"~black", 0},
        {"~dark_blue", 1},
        {"~dark_green", 2},
        {"~dark_cyan", 3},
        {"~dark_red", 4},
        {"~dark_magenta", 5},
        {"~dark_yellow", 6},
        {"~grey", 7},
        {"~dark_grey", 8},
        {"~blue", 9},
        {"~green", 10},
        {"~cyan", 11},
        {"~red", 12},
        {"~magenta", 13},
        {"~yellow", 14},
        {"~white", 15}
    };

    if (argc == 1 || (argc > 1 && strcmp(argv[1], "-h") == 0)) {
        cout << "" << endl;
        cout << "Usage: " << argv[0] << " [options] [foreground color] [background color] \"text to colorize\" [-s]" << endl;
        cout << "" << endl;
        cout << "Options:" << endl;
        cout << "  -h,    Display this help message" << endl;
        cout << "  -f,    Display current console text color" << endl;
        cout << "  -b,    Display current console background color" << endl;
        cout << "  -s,    As last argument, insert space between '[color] \"text\"' groups  " << endl;
        cout << "" << endl;
        cout << "Colors:" << endl;
        cout << "-----------" << endl;
        for(auto pair : colors) {
            cout << "  " << pair.first << endl;
        }
        cout << "-----------" << endl;
        cout << "  If no colors are specified, colorize acts as echo command" << endl;
        cout << "" << endl;
        cout << "Example:" << endl;
        cout << "  " << argv[0] << " -f" << endl;
        cout << "  " << argv[0] << " ~red \"text 1 \" ~yellow ~green \"text 2 \" \"text 3\"" << endl;
        cout << "  " << argv[0] << " ~red \"text 1 \" ~yellow ~green \"text 2 \" \"text 3\" -s" << endl;

        exit(EXIT_SUCCESS);
    }

    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!GetConsoleScreenBufferInfo(console, &coninfo)) { // get starting colors for reset
        cout << "============================================================" << endl;
        cout << "GetConsoleScreenBufferInfo, MAIN: " << GetLastError() << endl;
        cout << "============================================================" << endl;
        exit(EXIT_FAILURE);
    }
    conreset = coninfo.wAttributes;

    int bgColor = conreset & 0x00F0; // Mask for console background color
    bgColor >>= 4; // Shift right to get the color value

    if (strcmp(argv[1], "-b") == 0) {
        cout << color(colors, bgColor) << endl;
        exit(EXIT_SUCCESS);
    } else if (strcmp(argv[1], "-f") == 0) {
        int fgColor = conreset & 0x0F; // Mask to get foreground color
        cout << color(colors, fgColor) << endl;
        exit(EXIT_SUCCESS);
    }

    vector<pair<int, string>> parts;
    string a;
    string b;
    string c;
    int newattr;
    int i = 1;
    if (strcmp(argv[argc-1], "-s") == 0) {
        argc--;
        addSpase = true;
    }
    while (i < argc) {
        if (argc > i) {
            a = argv[i];
        }
        if (argc > (i+1)) {
            b = argv[i+1];
        }
        if (argc > (i+2)) {
            c = argv[i+2];
        }
        if (a.rfind("~", 0) == 0) {
            transform(a.begin(), a.end(), a.begin(), ::tolower);
            if (b.rfind("~", 0) == 0) {
                transform(b.begin(), b.end(), b.begin(), ::tolower);
                newattr = colors[b]<<4|colors[a];
                parts.emplace_back(newattr, c);
                i += 3;
            } else {
                newattr = bgColor<<4|colors[a];
                parts.emplace_back(newattr, b);
                i += 2;
            }
        } else {
            parts.emplace_back(conreset, a);
            i++;
        }
    }

    colorize(parts);
    reset();

    exit(EXIT_SUCCESS);
}
