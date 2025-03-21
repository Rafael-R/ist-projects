#ifndef COMMANDS_H
#define COMMANDS_H

#include "../aux/auxiliar.h"

enum command {
    COMMAND_START,
    COMMAND_PLAY,
    COMMAND_GUESS,
    COMMAND_SCORE,
    COMMAND_HINT,
    COMMAND_STATE,
    COMMAND_QUIT,
    COMMAND_EXIT,
    COMMAND_REV,
    COMMAND_NOP
};

command get_command(std::string c) {
    if (c == "start" || c == "sg") return COMMAND_START;
    if (c == "play" || c == "pl") return COMMAND_PLAY;
    if (c == "guess" || c == "gw") return COMMAND_GUESS;
    if (c == "scoreboard" || c == "sb") return COMMAND_SCORE;
    if (c == "hint" || c == "h") return COMMAND_HINT;
    if (c == "state" || c == "st") return COMMAND_STATE;
    if (c == "quit") return COMMAND_QUIT;
    if (c == "exit") return COMMAND_EXIT;
    if (c == "rev") return COMMAND_REV;
    return COMMAND_NOP;
}

enum reply_status {
    STATUS_OK,
    STATUS_WIN,
    STATUS_DUP,
    STATUS_NOK,
    STATUS_OVR,
    STATUS_INV,
    STATUS_ERR
};

reply_status get_status(std::string s) {
    if (s == "OK") return STATUS_OK;
    if (s == "WIN") return STATUS_WIN;
    if (s == "DUP") return STATUS_DUP;
    if (s == "NOK") return STATUS_NOK;
    if (s == "OVR") return STATUS_OVR;
    if (s == "INV") return STATUS_INV;
    return STATUS_ERR;
}

int command_start(UDP client, 
                  std::string &word,
                  int &max_errors,
                  std::string argument,
                  std::string &PLID)
{
    std::string request, reply, s_status;
    std::stringstream s;
    int n_letters;

    if (!isPLID(argument)) {
        std::cout << "Invalid PLID." << std::endl;
        return 0;
    }

    request = "SNG " + argument + "\n";

    reply = client.communicate(request);

    s = std::stringstream(reply);
    s >> reply >> s_status >> n_letters >> max_errors;

    // Check status
    reply_status status = get_status(s_status);
    switch (status) {
        case STATUS_OK:
            PLID = argument;
            word = std::string(n_letters, '_');
            return 1;
        case STATUS_NOK:
            std::cout << "Player has an ongoing game." << std::endl;
            break;
        default:
            std::cout << "Invalid PLID." << std::endl;
            break;
    }
    return 0;
}

int command_play(UDP client,
                 std::string &word,
                 std::string argument,
                 std::string PLID,
                 int &trial)
{
    std::string request, reply, s_status;
    std::stringstream stream;
    int n, pos;

    if (!isLetter(argument)) {
        std::cout << "You must play a letter." << std::endl;
        return 3;
    }

    request = "PLG " + PLID + " " + argument + " " + std::to_string(trial) + "\n";

    reply = client.communicate(request);

    stream = std::stringstream(reply);
    stream >> reply >> s_status >> trial;

    // Check status
    reply_status status = get_status(s_status);
    switch (status) {
        case STATUS_OK:
            stream >> n;
            for (size_t i = 0; i < n; i++) {
                stream >> pos;
                word.replace(pos-1, 1, argument);
            }
            return 1;
        case STATUS_WIN:
            print_winner();
            return -1;
        case STATUS_DUP:
            std::cout << "Duplicate letter. Try again." << std::endl;
            return 1;
        case STATUS_NOK:
            return 2;
        case STATUS_OVR:
            print_looser();
            return -1;
        case STATUS_INV:
            std::cout << "Internal error. Try again." << std::endl;
            return 1;
        case STATUS_ERR:
            std::cout << "Error. Try again." << std::endl;
            break;
    }
    return 0;
}

int command_guess(UDP client,
                  std::string word,
                  std::string argument,
                  std::string PLID,
                  int &trial)
{
    std::string request, reply, s_status;
    std::stringstream stream;

    if (argument.length() != word.length() || !isWord(argument)) {
        std::cout << "Word must have " << word.length() << " letters." << std::endl;
        return 2;
    }
    
    request = "PWG " + PLID + " " + argument + " " + std::to_string(trial) + "\n";

    reply = client.communicate(request);

    stream = std::stringstream(reply);
    stream >> reply >> s_status >> trial;

    // Check status
    reply_status status = get_status(s_status);
    switch (status) {
        case STATUS_WIN:
            print_winner();
            return -1;
        case STATUS_DUP:
            std::cout << "Duplicate word. Try again." << std::endl;
            return 1;
        case STATUS_NOK:
            std::cout << "Missed." << std::endl;
            return 1;
        case STATUS_OVR:
            print_looser();
            return -2;
        case STATUS_INV:
            std::cout << "Internal error. Try again." << std::endl;
            return 1;
        case STATUS_ERR:
            std::cout << "Error. Try again." << std::endl;
            break;
    }
    return 0;
}

void command_scoreboard(char* hostname, char* port) {   
    TCP tcp;
    tcp.client(hostname, port);

    std::string request = "GSB\n";

    tcp.communicate(request, "EMPTY");
}

void command_hint(char* hostname, char* port, std::string PLID) {
    TCP tcp;
    tcp.client(hostname, port);

    std::string request = "GHL " + PLID + "\n";

    tcp.communicate(request, "NOK");
}

void command_state(char* hostname, char* port, std::string PLID) {
    TCP tcp;
    tcp.client(hostname, port);

    std::string request = "STA " + PLID + "\n";

    tcp.communicate(request, "NOK");
}

int command_quit(UDP client, std::string PLID) {
    std::string request, reply, s_status;
    std::stringstream stream;

    request = "QUT " + PLID + "\n";

    reply = client.communicate(request);

    stream = std::stringstream(reply);
    stream >> reply >> s_status;

    // Check status
    reply_status status = get_status(s_status);
    switch (status) {
        case STATUS_OK:
            std::cout << "Successfully quitted game." << std::endl;
            return 0;
        case STATUS_NOK:
            std::cout << "No ongoing game." << std::endl;
            break;
        case STATUS_ERR:
            std::cout << "Error. Try again." << std::endl;
            break;
    }
    return 1;
}

void command_rev(UDP client, std::string PLID) {
    std::string request, reply, word;
    std::stringstream stream;

    request = "REV " + PLID + "\n";

    reply = client.communicate(request);

    stream = std::stringstream(reply);
    stream >> reply >> word;

    std::cout << word << std::endl;
}

#endif
