package pt.tecnico.distledger.adminclient;

import pt.tecnico.distledger.adminclient.grpc.AdminService;

import java.util.Scanner;

public class CommandParser {

    private static final String SPACE = " ";
    private static final String ACTIVATE = "activate";
    private static final String DEACTIVATE = "deactivate";
    private static final String GET_LEDGER_STATE = "getLedgerState";
    private static final String GOSSIP = "gossip";
    private static final String HELP = "help";
    private static final String EXIT = "exit";

    private final AdminService adminService;
    public CommandParser(AdminService adminService) {
        this.adminService = adminService;
    }
    void parseInput() {

        Scanner scanner = new Scanner(System.in);
        boolean exit = false;

        while (!exit) {
            System.out.print("> ");
            String line = scanner.nextLine().trim();
            String cmd = line.split(SPACE)[0];

            try {
                switch (cmd) {
                    case ACTIVATE:
                        this.activate(line);
                        break;

                    case DEACTIVATE:
                        this.deactivate(line);
                        break;

                    case GET_LEDGER_STATE:
                        this.dump(line);
                        break;

                    case GOSSIP:
                        this.gossip(line);
                        break;

                    case HELP:
                        this.printUsage();
                        break;

                    case EXIT:
                        exit = true;
                        AdminClientMain.debug("Terminate application");
                        this.adminService.close();
                        break;

                    default:
                        break;
                }
            } catch (Exception e) {
                System.err.println(e.getMessage());
            }
        }
    }

    private void activate(String line){
        String[] split = line.split(SPACE);

        if (split.length != 2){
            this.printUsage();
            return;
        }

        String server = split[1];

        adminService.activate(server);
        AdminClientMain.debug("Activate server " + server);
    }

    private void deactivate(String line){
        String[] split = line.split(SPACE);

        if (split.length != 2){
            this.printUsage();
            return;
        }

        String server = split[1];

        adminService.deactivate(server);
        AdminClientMain.debug("Deactivate server " + server);
    }

    private void dump(String line){
        String[] split = line.split(SPACE);

        if (split.length != 2){
            this.printUsage();
            return;
        }

        String server = split[1];

        adminService.getLedgerState(server);
        AdminClientMain.debug("Get ledger of server " + server);
    }

    @SuppressWarnings("unused")
    private void gossip(String line){
        String[] split = line.split(SPACE);

        if (split.length != 2){
            this.printUsage();
            return;
        }

        String server = split[1];

        adminService.gossip(server);    // TODO: implement gossip command (only for Phase-3)
        AdminClientMain.debug("Gossip server " + server);
    }

    private void printUsage() {
        System.out.println("Usage:\n" +
                "- activate <server>\n" +
                "- deactivate <server>\n" +
                "- getLedgerState <server>\n" +
                "- gossip <server>\n" +
                "- exit\n");
    }

}
