package pt.tecnico.rec;

import org.junit.jupiter.api.Test;
import pt.tecnico.rec.grpc.Rec.Tag;
import pt.tecnico.rec.grpc.Rec.Type;
import pt.tecnico.rec.grpc.Rec.UserData;
import pt.tecnico.rec.grpc.Rec.WriteRequest;
import pt.tecnico.rec.grpc.Rec.ReadRequest;
import pt.tecnico.rec.grpc.Rec.ReadResponse;

import static org.junit.jupiter.api.Assertions.*;

public class WriteIT extends RecordIT{

    @Test
    public void writeUserOK() throws InterruptedException {
        ReadRequest readRequest = ReadRequest.newBuilder()
                .setType(Type.USER)
                .setRegistry("alice")
                .build();

        ReadResponse response = frontend.read(readRequest);

        Tag tag = response.getTag();

        WriteRequest request = WriteRequest.newBuilder()
                .setType(Type.USER)
                .setRegistry("alice")
                .setUser(UserData.newBuilder()
                        .setBalance(10)
                        .build())
                .setTag(tag)
                .build();

        frontend.write(request);

        readRequest = ReadRequest.newBuilder()
                .setType(Type.USER)
                .setRegistry("alice")
                .build();


        response = frontend.read(readRequest);

        assertEquals(10, response.getUser().getBalance());
        assertFalse(response.getUser().getState());
    }
}
