package pt.tecnico.distledger.namingserver;

import io.grpc.BindableService;
import io.grpc.Server;
import io.grpc.ServerBuilder;
import pt.tecnico.distledger.namingserver.grpc.NamingServerServiceImpl;

import java.io.IOException;

public class NamingServer {

    private static final boolean DEBUG_FLAG = (System.getProperty("debug") != null);
    private static final int PORT = 5001;

    public static void main(String[] args) throws IOException, InterruptedException {
        System.out.println(NamingServer.class.getSimpleName());

        final BindableService namingServerService = new NamingServerServiceImpl();

        // Create a new server to listen on port.
        Server server = ServerBuilder
                .forPort(PORT)
                .addService(namingServerService)
                .build();

        // Start the server.
        server.start();

        // Server threads are running in the background.
        System.out.println("Server started");

        // Do not exit the main thread. Wait until server is terminated.
        server.awaitTermination();
    }

    /**
     * Display debug messages when debug flag is given as a name server option
     *
     * @param debugMessage Error message to be displayed
     */
    public static void debug(String debugMessage) {
        if (DEBUG_FLAG)
            System.err.println("DEBUG: " + debugMessage);
    }
}
