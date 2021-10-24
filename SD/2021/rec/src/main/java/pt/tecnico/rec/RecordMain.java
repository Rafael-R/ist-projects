package pt.tecnico.rec;


import java.io.IOException;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

import io.grpc.BindableService;
import io.grpc.Server;
import io.grpc.ServerBuilder;
import pt.ulisboa.tecnico.sdis.zk.ZKNaming;
import pt.ulisboa.tecnico.sdis.zk.ZKNamingException;


public class RecordMain {
	
	public static void main(String[] args) throws IOException, InterruptedException, ZKNamingException {
		System.out.println(RecordMain.class.getSimpleName());
		
		// receive and print arguments
		System.out.printf("Received %d arguments%n", args.length);
		for (int i = 0; i < args.length; i++) {
			System.out.printf("arg[%d] = %s%n", i, args[i]);
		}

		if (args.length < 1) {
			System.err.println("Argument(s) missing!");
			System.err.printf("Usage: java %s port%n", RecordMain.class.getName());
			return;
		}

		String zoo_host = args[0];
		String zoo_port = args[1];
		String rec_host = args[2];
		String rec_port = args[3];
		int instance = Integer.parseInt(args[4]);

		ZKNaming zkNaming = null;
		String server_path = "/grpc/bicloin/rec/" + instance;

		try {
			zkNaming = new ZKNaming(zoo_host, zoo_port);
			zkNaming.rebind(server_path, rec_host, rec_port);

			ZKNaming finalZkNaming = zkNaming;
			Thread shutdownListener = new Thread() {
				public void run() {
					System.out.println("Caught signal");
					try {
						System.out.println("Unbinding rec from ZooKeeper");
						finalZkNaming.unbind(server_path, rec_host, rec_port);
					} catch (ZKNamingException e) {
						System.out.println("Could not unbind rec");
					}
				}
			};

			Runtime.getRuntime().addShutdownHook(shutdownListener);

			final BindableService rec_server = new RecServiceImpl();

			Server server = ServerBuilder.forPort(Integer.parseInt(rec_port)).addService(rec_server).build();
			server.start();
			System.out.format("Replica %d starting...%n", instance);
			server.awaitTermination();
		} catch (ZKNamingException e) {
			e.printStackTrace();
		} finally {
			if (zkNaming != null) {
				zkNaming.unbind(server_path, rec_host, rec_port);
			}
		}
	}
	
}
