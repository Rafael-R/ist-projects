#ifndef GAME_PROTOCOL_H
#define GAME_PROTOCOL_H

#include "../aux/auxiliar.h"

enum request {
    SNG,
    PLG,
    PWG,
    QUT,
    REV,
    GSB,
    GHL,
    STA,
    KILLGAME,
    KILLPDIR,
    ERR
};

request process_request(char *buffer, std::string &arguments) {
    std::string command;
    std::string input(buffer);
    std::stringstream ss(input);
    ss >> command;
    arguments = input.length() > command.length() ? 
                input.substr(command.length() + 1) : "";
    if (command == "SNG") return SNG;
    if (command == "PLG") return PLG;
    if (command == "PWG") return PWG;
    if (command == "QUT") return QUT;
    if (command == "REV") return REV;
    if (command == "GSB") return GSB;
    if (command == "GHL") return GHL;
    if (command == "STA") return STA;
    if (command == "KILLGAME") return KILLGAME;
    if (command == "KILLPDIR") return KILLPDIR;
    return ERR;
}

std::string start(std::string arguments, std::string wordfile, int n)
{
    if (arguments.length() != 7 || arguments.back() != '\n')
        return "RSG ERR\n";

    std::string PLID = arguments.substr(0,6);
    if (!isPLID(PLID)) { return "RSG ERR\n"; }

    std::string filename = get_filename(PLID);
    if (exists_file(filename)) { return "RSG NOK\n"; }

    std::string word_hint = random_line(wordfile, n);
    std::string word = word_hint.substr(0, word_hint.find(" "));
    int length = word.length();
    int max_errors = get_max_errors(length);

    std::ofstream file(filename);
    file << word_hint << " " << max_errors << "\n";
    file.close();

    std::string reply = "RSG OK " + std::to_string(length) + " " 
                      + std::to_string(max_errors) + "\n";

    return reply;
}

std::string play(std::string arguments)
{
    if (arguments.back() != '\n') { return "RLG ERR\n"; }
    
    std::string PLID, letter;
    int trial;
    std::stringstream ss(arguments);

    ss >> PLID >> letter >> trial;
    if (!isPLID(PLID) || !isLetter(letter)) { return "RLG ERR\n"; }

    std::string filename = get_filename(PLID);
    if (!exists_file(filename)) { return "RLG ERR\n"; }

    std::string word, hint;
    int max_errors;
    std::ifstream infile(filename);

    infile >> word >> hint >> max_errors;

    int count = 0, errors = 0;
    std::string code, play;
    while (infile >> code >> play >> errors) {
        count++;
        if (trial != count && play == letter) { return "RLG DUP " + trial + '\n'; }
        std::replace(word.begin(), word.end(), play[0], '_');
    }

    infile.close();

    if (trial == count && play == word) {
        return "RWG NOK " + std::to_string(count) + '\n';
    }

    if (trial != count + 1 || (trial == count && play != word))
        return "RLG INV " + std::to_string(count) + '\n';

    std::ofstream outfile(filename, std::ios_base::app);

    count++;
    if (word.find(letter[0]) == std::string::npos) {    // doesn't contains letter
        outfile << "T " << letter << " " << ++errors << "\n";
        outfile.close();
        if (errors > max_errors) {
            move_file_to_dir(PLID, filename, "F");
            return "RLG OVR " + std::to_string(count) + '\n';
        } else {
            return "RLG NOK " + std::to_string(count) + '\n';
        }
    }

    int n = std::count(word.begin(), word.end(), letter[0]);
    int empty = std::count(word.begin(), word.end(), '_');
    if (n + empty != word.length()) {   // not win
        outfile << "T " << letter << " " << errors << "\n";
        outfile.close();
        std::string reply = "RLG OK " + std::to_string(count) 
                          + " " + std::to_string(n);
        for(size_t i = 0; i < word.length(); i++)
            if(word[i] == letter[0])
                reply += " " + std::to_string(i + 1);
        return reply + "\n";
    }

    outfile << "T " << word << " " << errors << "\n";
    outfile.close();
    move_file_to_dir(PLID, filename, "W");
    return "RLG WIN " + std::to_string(count) + "\n";
}

std::string guess(std::string arguments)
{
    if (arguments.back() != '\n') { return "RWG ERR\n"; }

    std::string PLID, word;
    int trial;
    std::stringstream ss(arguments);

    ss >> PLID >> word >> trial;
    if (!isPLID(PLID) || !isWord(word)) { return "RWG ERR\n"; }

    std::string filename = get_filename(PLID);
    if (!exists_file(filename)) { return "RWG ERR\n"; }

    std::string w, hint;
    int max_errors;
    std::ifstream infile(filename);

    infile >> w >> hint >> max_errors;

    int count = 0, errors = 0;
    std::string code, play;
    while (infile >> code >> play >> errors) {
        count++;
        if (trial != count && play == word) {
            infile.close();
            return "RWG DUP " + std::to_string(trial) + '\n'; 
        }
    }
    infile.close();

    if (trial == count && play == word)
        return "RWG NOK " + std::to_string(count) + '\n';

    if (trial != count + 1 || (trial == count && play != word))
        return "RWG INV " + std::to_string(count) + '\n';

    std::ofstream outfile(filename, std::ios_base::app);

    count++;
    if (word != w) {
        outfile << "G " << word << " " << ++errors << "\n";
        outfile.close();
        if (errors > max_errors) {
            move_file_to_dir(PLID, filename, "F");
            return "RWG OVR " + std::to_string(count) + '\n';
        } else {
            return "RWG NOK " + std::to_string(count) + '\n';
        }
    }

    outfile << "G " << word << " " << errors << "\n";
    outfile.close();
    move_file_to_dir(PLID, filename, "W");
    return "RWG WIN " + std::to_string(count) + "\n";
}

std::string quit(std::string arguments) {
    std::string reply, PLID;

    if (arguments.length() == 7 && arguments[6] == '\n') {
        PLID = arguments.substr(0,6);
        if (!isPLID(PLID)) { return "RQT ERR\n"; }
    } else { return "RQT ERR\n"; }

    std::string oldname = get_filename(PLID);
    if (!exists_file(oldname)) { return "RQT NOK\n"; }

    move_file_to_dir(PLID, oldname, "Q");

    return "RQT OK\n";
}

void scoreboard() {}

std::string hint(std::string arguments, std::string &hintfile) {
    std::string reply, PLID;

    if (arguments.length() == 7 && arguments[6] == '\n') {
        PLID = arguments.substr(0,6);
        if (!isPLID(PLID)) { return "RHL NOK\n"; }
    } else { return "RHL NOK\n"; }

    std::string filename = get_filename(PLID);
    if (!exists_file(filename)) { return "RHL NOK\n"; }

    std::string word;
    std::ifstream file(filename);
    file >> word >> hintfile;
    file.close();

    return "RHL OK\n";
}

void state(std::string arguments) {}

void kill(std::string arguments, int flag) {
    std::string PLID, filename;

    if (arguments.length() == 7 && arguments[6] == '\n') {
        PLID = arguments.substr(0,6);
        if (!isPLID(PLID)) { return; }
    } else {
        return;
    }

    switch (flag) {
        case 0:
            filename = get_filename(PLID);
            break;
        case 1:
            filename = get_dirname(PLID);
            break;
    }

    std::string cmd = "rm -r " + filename;

    if (system(cmd.c_str()) < 0) 
        std::cerr << "Error deleting file(s)" << std::endl;
}

#endif
