package pt.tecnico.rec;


import io.grpc.StatusRuntimeException;
import pt.ulisboa.tecnico.sdis.zk.ZKNamingException;
import pt.tecnico.bicloin.frontend.OldRecFrontend;

//Ping imports
import pt.tecnico.rec.grpc.Rec.PingRequest;
import pt.tecnico.rec.grpc.Rec.PingResponse;

//Read imports

//Write imports


public class OldRecordTester {
	
	public static void main(String[] args) {
		System.out.println(OldRecordTester.class.getSimpleName());
		
		// receive and print arguments
		System.out.printf("Received %d arguments%n", args.length);
		for (int i = 0; i < args.length; i++) {
			System.out.printf("arg[%d] = %s%n", i, args[i]);
		}

		if (args.length < 1) {
			System.err.println("Argument(s) missing!");
			System.err.printf("Usage: java %s port%n", OldRecordTester.class.getName());
			return;
		}

		String host = args[0];
		String port = args[1];

		OldRecFrontend frontend = new OldRecFrontend(host, port);

		PingResponse pingResponse = null;

		try {
			PingRequest pingRequest = PingRequest.newBuilder().setInstance("1").build();

			pingResponse = frontend.ping(pingRequest);

		} catch (StatusRuntimeException e) {
			System.out.println("Caught exception with description: " + e.getStatus().getDescription());
		} catch (ZKNamingException zke) {
			System.out.println("Caught exception with description: " + zke.getMessage());
		}
		System.out.println(pingResponse);

	}
}
