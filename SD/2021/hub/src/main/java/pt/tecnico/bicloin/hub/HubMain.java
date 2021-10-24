package pt.tecnico.bicloin.hub;


import java.io.FileReader;
import java.io.IOException;
import java.util.List;
import com.opencsv.bean.CsvToBeanBuilder;
import io.grpc.BindableService;
import io.grpc.Server;
import io.grpc.ServerBuilder;
import pt.ulisboa.tecnico.sdis.zk.ZKNaming;
import pt.ulisboa.tecnico.sdis.zk.ZKNamingException;


public class HubMain {
	
	public static void main(String[] args) throws IOException, InterruptedException, ZKNamingException {
		System.out.println(HubMain.class.getSimpleName());
		
		// receive and print arguments
		System.out.printf("Received %d arguments%n", args.length);
		for (int i = 0; i < args.length; i++) {
			System.out.printf("arg[%d] = %s%n", i, args[i]);
		}

		if (args.length < 1) {
			System.err.println("Argument(s) missing!");
			System.err.printf("Usage: java %s port%n", HubMain.class.getName());
			return;
		}

		String zoo_host = args[0];
		String zoo_port = args[1];
		String hub_host = args[2];
		String hub_port = args[3];
		int instance = Integer.parseInt(args[4]);
		String users_file = args[5];
		String stations_file = args[6];

		ZKNaming zkNaming = null;
		String server_path = "/grpc/bicloin/hub/" + instance;

		try {
			zkNaming = new ZKNaming(zoo_host, zoo_port);
			zkNaming.rebind(server_path, hub_host, hub_port);

			List<User> users = new CsvToBeanBuilder(new FileReader(users_file))
					.withType(User.class)
					.build()
					.parse();

			List<Station> stations = new CsvToBeanBuilder(new FileReader(stations_file))
					.withType(Station.class)
					.build()
					.parse();

			final BindableService hub_server;
			if (args.length == 8 && args[7].equals("initRec")) {
				hub_server = new HubServiceImpl(zkNaming, instance, users, stations, true);
			} else {
				hub_server = new HubServiceImpl(zkNaming, instance, users, stations, false);
			}

			Server server = ServerBuilder.forPort(Integer.parseInt(hub_port)).addService(hub_server).build();
			server.start();
			System.out.println("Hub starting...");
			server.awaitTermination();
		} catch (ZKNamingException e) {
			e.printStackTrace();
		} finally {
			if (zkNaming != null) {
				zkNaming.unbind(server_path, hub_host, hub_port);
			}
		}
	}
	
}
