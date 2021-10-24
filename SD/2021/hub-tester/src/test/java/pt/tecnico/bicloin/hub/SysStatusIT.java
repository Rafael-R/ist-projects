package pt.tecnico.bicloin.hub;


import org.junit.jupiter.api.Test;
import pt.tecnico.bicloin.hub.grpc.Hub.StatusRequest;
import pt.tecnico.bicloin.hub.grpc.Hub.StatusResponse;

import static org.junit.jupiter.api.Assertions.assertEquals;


public class SysStatusIT extends HubIT {

    @Test
    public void sysStatusOKTest() {
        StatusRequest request = StatusRequest.newBuilder().build();

        StatusResponse stat_res = frontend.sysStatus(request);

        String[] statusList = stat_res.getStatusList().toArray(new String[0]);

        String response = "";

        for (String status: statusList) {
            response += status + " | ";
        }

        response = response.substring(0, response.length() - 3);

        String expectedResponse = "/grpc/bicloin/hub/1 UP | /grpc/bicloin/rec/1 UP | /grpc/bicloin/rec/2 UP";

        assertEquals(expectedResponse, response);
    }
}
