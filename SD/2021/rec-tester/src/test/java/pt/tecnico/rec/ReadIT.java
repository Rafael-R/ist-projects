package pt.tecnico.rec;

import org.junit.jupiter.api.Test;
import pt.tecnico.rec.grpc.Rec.Type;
import pt.tecnico.rec.grpc.Rec.ReadRequest;
import pt.tecnico.rec.grpc.Rec.ReadResponse;

import static org.junit.jupiter.api.Assertions.*;

public class ReadIT extends RecordIT{

    @Test
    public void readUserOK() throws InterruptedException {
        ReadRequest request = ReadRequest.newBuilder()
                                         .setType(Type.USER)
                                         .setRegistry("User")
                                         .build();

        ReadResponse response = frontend.read(request);

        assertEquals(0, response.getUser().getBalance());
        assertFalse(response.getUser().getState());
    }

    @Test
    public void emptyReadTest() throws InterruptedException {
        ReadRequest request = ReadRequest.newBuilder()
                .setType(Type.USER)
                .setRegistry("")
                .build();

        ReadResponse response = frontend.read(request);

        ReadResponse expectedResponse = ReadResponse.newBuilder().build();

        assertEquals(expectedResponse, response);
    }
}
