package pt.tecnico.distledger.server;

import io.grpc.Server;
import io.grpc.ServerBuilder;
import io.grpc.BindableService;
import pt.tecnico.distledger.namingserver.grpc.NamingServerService;
import pt.tecnico.distledger.server.domain.ServerState;
import pt.tecnico.distledger.server.grpc.AdminServiceImpl;
import pt.tecnico.distledger.server.grpc.CrossServerServiceImpl;
import pt.tecnico.distledger.server.grpc.UserServiceImpl;

import java.io.IOException;

public class ServerMain {

    private static final boolean DEBUG_FLAG = (System.getProperty("debug") != null);

    public static String QUALIFIER;
    private static final String SERVICE = "DistLedger";

    public static void main(String[] args) throws IOException, InterruptedException {

        System.out.println(ServerMain.class.getSimpleName());

        // receive and print arguments
        System.out.printf("Received %d arguments%n", args.length);
        for (int i = 0; i < args.length; i++) {
            System.out.printf("arg[%d] = %s%n", i, args[i]);
        }

        // check arguments
        if (args.length < 2) {
            System.err.println("Argument(s) missing!");
            System.err.printf("Usage: java %s port qualifier%n", ServerMain.class.getName());
            return;
        }

        // Set server address
        final int port = Integer.parseInt(args[0]);
        final String ADDRESS = "localhost:" + port;

        // Set server qualifier
        QUALIFIER = args[1];

        final NamingServerService namingServer = new NamingServerService();

        final ServerState serverState = new ServerState();

        final BindableService adminImpl = new AdminServiceImpl(serverState);
        final BindableService userImpl = new UserServiceImpl(serverState);
        final BindableService crossImpl = new CrossServerServiceImpl(serverState);

        // Create a new server
        Server server = ServerBuilder
            .forPort(port)
            .addService(adminImpl)
            .addService(userImpl)
            .addService(crossImpl)
            .build();

        // Start the server.
        server.start();

        // Server threads are running in the background.
        System.out.println("Server started");

        // Register server in naming server
        namingServer.register(SERVICE, QUALIFIER, ADDRESS);

        // Delete server from naming server on Ctrl-C
        Runtime.getRuntime().addShutdownHook(new Thread() {
            @Override
            public void run() { namingServer.delete(SERVICE, ADDRESS); }
        });

        // Do not exit the main thread. Wait until server is terminated.
        server.awaitTermination();
    }

    /**
     * Display debug messages when debug flag is given as a server option
     *
     * @param debugMessage Error message to be displayed
     */
    public static void debug(String debugMessage) {
        if (DEBUG_FLAG)
            System.err.println("DEBUG: " + debugMessage);
    }
}
