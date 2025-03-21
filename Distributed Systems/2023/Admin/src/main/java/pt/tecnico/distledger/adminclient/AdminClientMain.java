package pt.tecnico.distledger.adminclient;

import pt.tecnico.distledger.adminclient.grpc.AdminService;

public class AdminClientMain {

    // Debug mode flag
    private static final boolean DEBUG_FLAG = (System.getProperty("debug") != null);

    public static void main(String[] args) {

        System.out.println(AdminClientMain.class.getSimpleName());

        CommandParser parser = new CommandParser(new AdminService());
        debug("Create admin client service");

        parser.parseInput();
    }

    /**
     * Display debug messages when debug flag is given as a client option
     *
     * @param debugMessage Error message to be displayed
     */
    public static void debug(String debugMessage) {
        if (DEBUG_FLAG)
            System.err.println("DEBUG: " + debugMessage);
    }
}
