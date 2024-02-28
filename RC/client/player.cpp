#include "commands.h"

#define DEFAULT_PORT "58007"

using namespace std;


command parse_input(string input, string &argument);

void exit_handler(int s);


bool exit_game = false;

int main(int argc, char* argv[])
{
    struct sigaction sigIntHandler;
    char hostname[128], port[6] = DEFAULT_PORT;
    int trial, errors, status;
    string input, argument, PLID, word;
    UDP udp;
    command c;
    bool game = false;

    // Ctrl+C signal handling
    sigIntHandler.sa_handler = exit_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    // Argument parsing
    if (argc == 5) {
        if (strcmp(argv[1], "-n") == 0 && strcmp(argv[3], "-p") == 0) {
            strcpy(hostname, argv[2]);
            strcpy(port, argv[4]);
        } else 
        if (strcmp(argv[1], "-p") == 0 && strcmp(argv[3], "-n") == 0) {
            strcpy(port, argv[2]);
            strcpy(hostname, argv[4]);
        }
    } else {
        if (gethostname(hostname, 128) == -1) exit(1);  // error
    }

    // Connect to socket [UDP]
    udp.client(hostname, port);

    // print_instructions();    // TODO

    // Commands
    while (!exit_game)
    {
        getline(cin, input);
        c = parse_input(input, argument);

        if (c == COMMAND_START || c == COMMAND_PLAY || c == COMMAND_GUESS) {
            if (argument == "") {
                cout << "Missing argument: command [argument]" << endl;
                continue; 
            }
        }

        switch (c) {
            case COMMAND_START:
                if (command_start(udp, word, errors, argument, PLID)) {
                    cout << "New game started." << endl << "Guess " << 
                    word.length() << " letter word: ";
                    print_word(word);
                    cout << endl << "You can make " << errors << " errors." << endl;
                    trial = 1;
                    game = true;
                }
                break;
            case COMMAND_PLAY:
                status = command_play(udp, word, argument, PLID, trial);
                if (status > 0) {
                    if (status == 1) trial++;
                    else if (status == 2) { trial++; errors--; }
                    cout << "Word: ";
                    print_word(word);
                    cout << "[" << errors << " errors left]" << endl;
                } else
                if (status < 0) {
                    game = false; 
                }
                break;
            case COMMAND_GUESS:
                status = command_guess(udp, word, argument, PLID, trial);
                if (status > 0) {
                    if (status == 1) { trial++; errors--; }
                    cout << "Word: ";
                    print_word(word);
                    cout << "[" << errors << " errors left]" << endl;
                } else
                if (status < 0) {
                    game = false; 
                }
                break;
            case COMMAND_SCORE:
                command_scoreboard(hostname, port);
                break;
            case COMMAND_HINT:
                command_hint(hostname, port, PLID);
                break;
            case COMMAND_STATE:
                command_state(hostname, port, PLID);
                break;
            case COMMAND_QUIT:
                if (command_quit(udp, PLID) == 0) { game = false; }
                break;
            case COMMAND_EXIT:
                exit_game = true;
                break;
            case COMMAND_REV:
                command_rev(udp, PLID);
                break;
            case COMMAND_NOP:
                break;
        }
    }

    if (game) { command_quit(udp, PLID); }
    
    cout << "Closing..." << endl;
    udp.close_connection();
    
    return 0;
}


command parse_input(string input, string &argument) {
    stringstream s(input);
    string command;

    s >> command >> argument;
    return get_command(command);
};


void exit_handler(int s) {
    if (s == 2) {   // Ctrl+C
        cout << endl;
        exit_game = true;
    }
}
