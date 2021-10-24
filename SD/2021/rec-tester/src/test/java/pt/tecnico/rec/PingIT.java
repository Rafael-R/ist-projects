package pt.tecnico.rec;

import java.util.List;

import org.junit.jupiter.api.Test;
import pt.tecnico.rec.grpc.Rec.PingResponse;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class PingIT extends RecordIT{

    @Test
    public void pingOKTest() throws InterruptedException {
        List<PingResponse> responses = frontend.ping();

        String[] expectedResponses = {"/grpc/bicloin/rec/1 UP", "/grpc/bicloin/rec/2 UP"};

        int i = 0;
        for (PingResponse pingResponse : responses) {
            String response = pingResponse.getResponse();
            assertEquals(expectedResponses[i], response);
            i++;
        }
    }
}
