package pt.tecnico.bicloin.hub;


import org.junit.jupiter.api.*;
import pt.tecnico.bicloin.frontend.HubFrontend;
import pt.tecnico.bicloin.hub.grpc.Hub;
import pt.ulisboa.tecnico.sdis.zk.ZKNamingException;


public class HubIT {

	// static members
	private static String _host;
	private static String _port;
	public static HubFrontend frontend;
	public static Hub.InitRecRequest request;


	// one-time initialization and clean-up
	@BeforeAll
	public static void oneTimeSetUp(){
		_host = "localhost";
		_port = "2181";
		try {
			frontend = new HubFrontend(_host, _port);
		} catch (Exception e) {
			System.out.println("Exception caught");
		}
		request = Hub.InitRecRequest.newBuilder().build();
	}
	
	@AfterAll
	public static void oneTimeTearDown() {
		frontend.initRec(request);
	}
	
	// initialization and clean-up for each test
	
	@BeforeEach
	public void setUp() {

	}
	
	@AfterEach
	public void tearDown() {

	}
		
	// tests 
	
	@Test
	public void test() {
		
		
	}

}
