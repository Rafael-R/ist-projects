#ifndef AUXILIAR_H
#define AUXILIAR_H

#include "networking.h"
#include <algorithm>

void print_word(std::string word) {
    for (size_t i = 0; i < word.length(); i++)
        std::cout << word[i] << " ";
};

bool exists_file(const std::string& name) {
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}

int count_lines(std::string filename) {
    int count = 0;
    std::string line;
    std::ifstream file(filename);

    while (getline(file, line)) ++count;

    return count;
}

std::string random_line(std::string filename, int n) {
    int index = rand() % n; 
    std::string line;
    std::ifstream file(filename);
    for (size_t i = 0; i < index; i++) 
        getline(file, line);
    file.close();
    return line;
}

int get_max_errors(int length) {
    int max_errors;
    if (length <= 6) { max_errors = 7; } 
    else if (length >= 11) { max_errors = 9; }
    else { max_errors = 8; }
    return max_errors;
}

const std::string get_filename(std::string PLID) {
    return "server/games/GAMES_" + PLID + ".txt";
}

const std::string get_dirname(std::string PLID) {
    return "server/games/" + PLID + "/";
}

const std::string get_timestamp() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", &tstruct);
    return buf;
}

void move_file_to_dir(std::string PLID, std::string filename, std::string code) {
    std::string dir = get_dirname(PLID);
    int status = mkdir(dir.c_str(), 0777);
    if (status != 0 && errno != EEXIST)
        std::cerr << "Error creating directory" << std::endl;   // error

    std::string newname = dir + get_timestamp() + "_" + code + ".txt";
    if (std::rename(filename.c_str(), newname.c_str()) != 0) 
        std::cerr << "Error renaming file" << std::endl;    // error
}

bool isPLID(std::string argument) {
    int number = std::stoi(argument);
    if (number < 0 || number > 999999)
        return false;
    return true;
}

void toLower(std::string &s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
}

bool isLetter(std::string &argument) {
    if (argument.length() == 1 && std::isalpha(argument[0])) {
        toLower(argument);
        return true;
    }
    return false;
}

bool isWord(std::string &argument) {
    for(int i = 0; i < argument.size(); i++) {
        if (!isalpha(argument[i]))
            return false;
    }
    toLower(argument);
    return true;
}

void print_instructions() {
    std::cout << "###################################################" << std::endl;
    std::cout << "# #" << std::endl;
    std::cout << "# #" << std::endl;
    std::cout << "# #" << std::endl;
    std::cout << "# #" << std::endl;
    std::cout << "# #" << std::endl;
    std::cout << "# #" << std::endl;
    std::cout << "###################################################" << std::endl;
}

void print_winner() {
    std::cout << " __      __  ___   _  _   _  _   ___   ___ " << std::endl;
    std::cout << " \\ \\    / / |_ _| | \\| | | \\| | | __| | _ \\" << std::endl;
    std::cout << "  \\ \\/\\/ /   | |  | .` | | .` | | _|  |   /" << std::endl;
    std::cout << "   \\_/\\_/   |___| |_|\\_| |_|\\_| |___| |_|_\\" << std::endl;
}

void print_looser() {
    std::cout << "   ___     _     __  __   ___      ___   __   __  ___   ___ " << std::endl;
    std::cout << "  / __|   /_\\   |  \\/  | | __|    / _ \\  \\ \\ / / | __| | _ \\" << std::endl;
    std::cout << " | (_ |  / _ \\  | |\\/| | | _|    | (_) |  \\ V /  | _|  |   /" << std::endl;
    std::cout << "  \\___| /_/ \\_\\ |_|  |_| |___|    \\___/    \\_/   |___| |_|_\\" << std::endl;
}

std::ifstream::pos_type get_filesize(std::string filename) {
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); 
}

#endif