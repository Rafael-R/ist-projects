package pt.tecnico.bicloin.app;

import java.util.Arrays;
import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.Scanner;
import java.util.concurrent.TimeUnit;
import pt.ulisboa.tecnico.sdis.zk.ZKNamingException;


public class AppMain {

	public static void help() {
		System.out.println("App usage:");
		System.out.println("'balance': returns your current balance in BIC");
		System.out.println("'top-up <amount>': increases your current balance by <amount>*10 BIC");
		System.out.println("'tag <lat> <long> <name>': creates a tag with name <name>, latitude <lat> and longitude <long>");
		System.out.println("'move <tag_name>': moves your position to <tag_name>'s position if it exists");
		System.out.println("'move <lat> <long>': moves your position to latitude <lat> and longitude <long>");
		System.out.println("'at': returns your current position");
		System.out.println("'scan <num>': returns information on the <num> closest stations or the information off all stations if <num> > number of stations");
		System.out.println("'info <station_id>': returns information on the station with id <station_id>");
		System.out.println("'bike-up <station_id>': rents bike form the station with id <station_id> and removes 10 BIC form your account");
		System.out.println("'bike-down <station_id>': returns bike to the station with id <station_id> and adds its prize to your account");
		System.out.println("'ping': pings the hub the app is connected to");
		System.out.println("'sys_status': returns the status of every hub and rec in the system");
		System.out.println("'exit': exit app");
	}
	
	public static void main(String[] args) throws ZKNamingException {
		System.out.println(AppMain.class.getSimpleName());
		
		// receive and print arguments
		System.out.printf("Received %d arguments%n", args.length);
		for (int i = 0; i < args.length; i++) {
			System.out.printf("arg[%d] = %s%n", i, args[i]);
		}

		if (args.length < 6) {
			System.err.println("Argument(s) missing!");
			System.err.printf("Usage: java %s port%n", AppMain.class.getName());
			return;
		} else if (args.length > 6) {
			System.err.println("Too many argument(s)!");
			System.err.printf("Usage: java %s port%n", AppMain.class.getName());
			return;
		}

		String zoo_host = args[0];
		String zoo_port = args[1];
		String user_id = args[2];;
		String number = args[3];
		double user_lat;
		double user_long;


		try {
			user_lat = Double.parseDouble(args[4]);
			user_long = Double.parseDouble(args[5]);
		} catch (NumberFormatException e) {
			System.err.println("Invalid argument(s) type!");
			e.printStackTrace();
			return;
		}

		Scanner in = new Scanner(System.in);
		String[] arrOfStr;
		Iterator<String> iter;

		try {
			App appClient = new App(zoo_host, zoo_port, user_id, number, user_lat, user_long);

			String command;
			boolean exit = false;
			while (!exit) {
				System.out.print("> ");
				if (in.hasNext()) {
					arrOfStr = in.nextLine().split(" ", -2);
					iter = Arrays.stream(arrOfStr).iterator();
					command = iter.next();

					switch (command) {
						case "#":
							break;
						case "zzz":
							if (iter.hasNext()) {
								int d = Integer.parseInt(iter.next());
								TimeUnit.SECONDS.sleep(d);
							} else {
								System.out.println("  ^ Error: invalid command");
							}
							break;
						case "balance":
							System.out.println(appClient.balance());
							break;
						case "top-up":
							if (iter.hasNext()) {
								System.out.println(appClient.topUp(iter.next()));
							} else {
								System.out.println("'top-up <amount>': increases your current balance by <amount>*10 BIC");
							}
							break;
						case "tag":
							if (arrOfStr.length == 4) {
								System.out.println(appClient.tag(iter.next(), iter.next(), iter.next()));
							} else {
								System.out.println("'tag <lat> <long> <name>': creates a tag with name <name>, latitude <lat> and longitude <long>");
							}
							break;
						case "move":
							if (iter.hasNext()) {
								String move = iter.next();
								if (iter.hasNext()) {
									System.out.println(appClient.move(move, iter.next()));
								} else {
									System.out.println(appClient.moveToTag(move));
								}
							} else {
								System.out.println("'move <tag_name>': moves your position to <tag_name>'s position if it exists");
								System.out.println("'move <lat> <long>': moves your position to latitude <lat> and longitude <long>");
							}
							break;
						case "at":
							System.out.println(appClient.at());
							break;
						case "scan":
							if (iter.hasNext()) {
								System.out.print(appClient.scan(iter.next()));
							} else {
								System.out.println("'scan <num>': returns information on the <num> closest stations or the information off all stations if <num> > number of stations");
							}
							break;
						case "info":
							if (iter.hasNext()) {
								System.out.println(appClient.info(iter.next()));
							} else {
								System.out.println("'info <station_id>': returns information on the station with id <station_id>");
							}
							break;
						case "bike-up":
							if (iter.hasNext()) {
								System.out.println(appClient.bikeUp(iter.next()));
							} else {
								System.out.println("'bike-up <station_id>': rents bike form the station with id <station_id> and removes 10 BIC form your account");
							}
							break;
						case "bike-down":
							if (iter.hasNext()) {
								System.out.println(appClient.bikeDown(iter.next()));
							} else {
								System.out.println("'bike-down <station_id>': returns bike to the station with id <station_id> and adds its prize to your account");
							}
							break;
						case "ping":
							System.out.println(appClient.ping());
							break;
						case "sys_status":
							System.out.println(appClient.sysStatus());
							break;
						case "exit":
							exit = true;
							break;
						case "help":
							help();
							break;
						default:
							System.out.println("  ^ Error: invalid command");
					}
				} else {
					exit = true;
				}
			}
		} catch (NoSuchElementException e) {
			System.out.println("  ^ Error: invalid use of command, use command \"help\" for App usage information");
		} catch (InterruptedException ie){
			ie.printStackTrace();
		}
	}
}
