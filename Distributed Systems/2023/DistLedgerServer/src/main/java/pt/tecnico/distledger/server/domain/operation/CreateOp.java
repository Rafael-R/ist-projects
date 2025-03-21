package pt.tecnico.distledger.server.domain.operation;

import java.util.List;

public class CreateOp extends Operation {

    public CreateOp(String account, List<Integer> prevTS, List<Integer> ts) {
        super(account, prevTS, ts);
    }

}
