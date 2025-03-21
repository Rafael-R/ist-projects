package pt.tecnico.rec;

import org.junit.jupiter.api.*;
import pt.tecnico.rec.grpc.Rec;
import pt.ulisboa.tecnico.sdis.zk.ZKNamingException;
import pt.tecnico.bicloin.frontend.RecFrontend;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class RecordIT {
	
	// static members
	private static String _host;
	private static String _port;
	public static RecFrontend frontend;
	public static Rec.Empty empty_obj;


	// one-time initialization and clean-up
	@BeforeAll
	public static void oneTimeSetUp(){
		_host = "localhost";
		_port = "2181";
		frontend = new RecFrontend(_host, _port, 1);
	}
	
	@AfterAll
	public static void oneTimeTearDown() {
		
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
