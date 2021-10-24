package pt.tecnico.rec;


import io.grpc.StatusRuntimeException;
import pt.tecnico.bicloin.frontend.RecFrontend;

//Ping imports
import pt.tecnico.rec.grpc.Rec;
import pt.tecnico.rec.grpc.Rec.PingResponse;

import java.util.ArrayList;
import java.util.List;

//Read imports

//Write imports


public class RecordTester {

    public static void main(String[] args) {
        System.out.println(RecordTester.class.getSimpleName());

        // receive and print arguments
        System.out.printf("Received %d arguments%n", args.length);
        for (int i = 0; i < args.length; i++) {
            System.out.printf("arg[%d] = %s%n", i, args[i]);
        }

        if (args.length < 1) {
            System.err.println("Argument(s) missing!");
            System.err.printf("Usage: java %s port%n", RecordTester.class.getName());
            return;
        }

        String host = args[0];
        String port = args[1];

        RecFrontend frontend = new RecFrontend(host, port, 1);

        try {
            List<PingResponse> responses = frontend.ping();

            for (PingResponse response : responses) {
                System.out.println(response.getResponse());
            }
        } catch (StatusRuntimeException e) {
            System.out.println("Caught exception " + e + " with description: " + e.getStatus().getDescription());
        } catch (InterruptedException e) {
            System.out.println("Caught exception " + e + " with message: " + e.getMessage());
        }
    }
}
