package pt.tecnico.bicloin.frontend;

import pt.tecnico.rec.grpc.Rec.ReadResponse;

import java.util.ArrayList;
import java.util.List;

public class ResponseCollector<C> {
    List<C> _responses;

    public ResponseCollector() {
        _responses = new ArrayList<>();
    }

    public void addResponse(C r) {
        _responses.add(r);
    }

    public List<C> getResponses() {
        return _responses;
    }
}
