package pt.tecnico.distledger.userclient;

import pt.tecnico.distledger.userclient.grpc.UserService;

import java.util.Scanner;

public class CommandParser {

    private static final String SPACE = " ";
    private static final String CREATE_ACCOUNT = "createAccount";
    private static final String TRANSFER_TO = "transferTo";
    private static final String BALANCE = "balance";
    private static final String HELP = "help";
    private static final String EXIT = "exit";

    private final UserService userService;

    public CommandParser(UserService userService) {
        this.userService = userService;
    }

    void parseInput() {

        Scanner scanner = new Scanner(System.in);
        boolean exit = false;

        while (!exit) {
            System.out.print("> ");
            String line = scanner.nextLine().trim();
            String cmd = line.split(SPACE)[0];

            try{
                switch (cmd) {
                    case CREATE_ACCOUNT:
                        this.createAccount(line);
                        break;

                    case TRANSFER_TO:
                        this.transferTo(line);
                        break;

                    case BALANCE:
                        this.balance(line);
                        break;

                    case HELP:
                        this.printUsage();
                        break;

                    case EXIT:
                        exit = true;
                        UserClientMain.debug("Terminate application");
                        this.userService.close();
                        break;

                    default:
                        break;
                }
            }
            catch (Exception e){
                System.err.println(e.getMessage());
            }
        }
    }

    private void createAccount(String line){
        String[] split = line.split(SPACE);

        if (split.length != 3){
            this.printUsage();
            return;
        }

        String server = split[1];
        String username = split[2];

        // Create user account
        userService.createAccount(server, username);
        UserClientMain.debug("Create account '" + username + "'");
    }

    private void balance(String line){
        String[] split = line.split(SPACE);

        if (split.length != 3){
            this.printUsage();
            return;
        }
        String server = split[1];
        String username = split[2];

        // Check user account balance
        userService.balance(server, username);
        UserClientMain.debug("Check account balance of '" + username + "'");
    }

    private void transferTo(String line){
        String[] split = line.split(SPACE);

        if (split.length != 5){
            this.printUsage();
            return;
        }
        String server = split[1];
        String from = split[2];
        String dest = split[3];
        Integer amount = Integer.valueOf(split[4]);

        // Transfer funds
        userService.transferTo(server, from, dest, amount);
        UserClientMain.debug("Transfer from '" + from + "' to '" + dest + "' amount '" + amount + "'");
    }

    private void printUsage() {
        System.out.println("Usage:\n" +
                        "- createAccount <server> <username>\n" +
                        "- balance <server> <username>\n" +
                        "- transferTo <server> <username_from> <username_to> <amount>\n" +
                        "- exit\n");
    }
}
