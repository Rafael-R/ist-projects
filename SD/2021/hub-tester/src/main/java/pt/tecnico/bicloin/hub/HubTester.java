package pt.tecnico.bicloin.hub;


import io.grpc.StatusRuntimeException;
import pt.ulisboa.tecnico.sdis.zk.ZKNamingException;
import pt.tecnico.bicloin.hub.grpc.Hub.*;
import pt.tecnico.bicloin.frontend.HubFrontend;


public class HubTester {
	
	public static void main(String[] args) {
		System.out.println(HubTester.class.getSimpleName());
		
		// receive and print arguments
		System.out.printf("Received %d arguments%n", args.length);
		for (int i = 0; i < args.length; i++) {
			System.out.printf("arg[%d] = %s%n", i, args[i]);
		}

		if (args.length < 1) {
			System.err.println("Argument(s) missing!");
			System.err.printf("Usage: java %s port%n", HubTester.class.getName());
			return;
		}

		String host = args[0];
		String port = args[1];

		PingResponse pingResponse = null;

		try {
			HubFrontend frontend = new HubFrontend(host, port);

			PingRequest pingRequest = PingRequest.newBuilder()
					.setInputText("friend")
					.build();

			pingResponse = frontend.ping(pingRequest);

		} catch (StatusRuntimeException e) {
			System.out.println("Caught exception with description: " + e.getStatus().getDescription());
		}
		System.out.println(pingResponse);
	}
}
