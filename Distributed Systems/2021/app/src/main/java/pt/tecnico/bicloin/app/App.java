package pt.tecnico.bicloin.app;

import java.util.Map;
import java.util.HashMap;
import static java.lang.Math.min;
import pt.tecnico.bicloin.frontend.HubFrontend;
import pt.tecnico.bicloin.hub.grpc.Hub;
import pt.ulisboa.tecnico.sdis.zk.ZKNaming;
import pt.ulisboa.tecnico.sdis.zk.ZKNamingException;

public class App {
    private final String _name;
    private final String _number;
    private double _lat;
    private double _long;
    ZKNaming _zkNaming;
    private Map<String, Tag> _tags = new HashMap<>();

    public App(String host, String port, String user_id, String number, double coordinatesX, double coordinatesY) {
        _name = user_id;
        _number = number;
        _lat = coordinatesX;
        _long = coordinatesY;
        _zkNaming = new ZKNaming(host, port);
    }

    private String googleMapsLink(double latitude, double longitude) {
        return "https://www.google.com/maps/place/" + latitude + "," + longitude;
    }

    public String balance() {
        try {
            HubFrontend frontend = new HubFrontend(_zkNaming);

            Hub.BalanceRequest info_req = Hub.BalanceRequest.newBuilder().setUsername(_name).build();

            Hub.BalanceResponse info_res = frontend.balance(info_req);

            return _name + " " + info_res.getBalance() + " BIC";
        } catch (RuntimeException run_ex) {
            return "User is not registered.";
        }
    }

    public String topUp(String n) {
        try {
            HubFrontend frontend = new HubFrontend(_zkNaming);

            Hub.TopUpRequest info_req = Hub.TopUpRequest.newBuilder()
                    .setUsername(_name)
                    .setAmount(Integer.parseInt(n))
                    .setPhoneNumber(_number).build();

            Hub.TopUpResponse info_res = frontend.topUp(info_req);

            return _name + " " + info_res.getBalance() + " BIC";
        } catch (NumberFormatException num_e) {
            return "Usage: top-up <num>, <num> has to be an integer.";
        } catch (RuntimeException run_ex) {
            return "User does not exist or invalid amount.";
        }
    }

    public String tag(String t_lat, String t_long, String tag) {
        try {
            Tag new_tag = new Tag(Double.parseDouble(t_lat), Double.parseDouble(t_long), tag);
            _tags.put(tag, new_tag);
            return "OK";
        } catch (NumberFormatException num_e) {
            return "Usage: tag <lat> <long> <name>, <lat> and <long> must be doubles.";
        }
    }

    public String moveToTag(String tag) {
        if (_tags.get(tag) == null) {
            return "Error: tag not found";
        }
        Tag tTag = _tags.get(tag);
        _lat = tTag.getLatitude();
        _long = tTag.getLongitude();
        return at();
    }

    public String move(String m_lat, String m_long) {
        try {
            _lat = Double.parseDouble(m_lat);
            _long = Double.parseDouble(m_long);
            return at();
        } catch (NumberFormatException num_e) {
            return "Usage: move <lat> <long>, <lat> and <long> must be doubles.";
        }
    }

    public String at() {
        return _name + " em " + googleMapsLink(_lat, _long);
    }

    public String scan(String n) {
        try {
            int num_stations = Integer.parseInt(n);

            HubFrontend frontend = new HubFrontend(_zkNaming);

            Hub.LocateStationRequest info_req = Hub.LocateStationRequest.newBuilder()
                    .setLat(_lat)
                    .setLong(_long)
                    .setNumStations(num_stations)
                    .build();

            Hub.LocateStationResponse info_res = frontend.locateStation(info_req);

            int num_stations_returned = info_res.getStationCount();
            num_stations = min(num_stations, num_stations_returned);

            String response = "";
            for (int i = 0; i < num_stations; i++) {
                Hub.StationData station = info_res.getStation(i);

                response += station.getName() + ", lat " + station.getLat() + ", " + station.getLong() + " long, "
                        + station.getDocks() + " docas, " + station.getPrize() + " BIC prémio, " + station.getBikes()
                        + " bicicletas, a " + station.getDistance() + " metros.\n";
            }
            return response;
        } catch (NumberFormatException num_e) {
            return "Usage: scan <num_stations>, <num_stations> has to be an integer.";
        }
    }

    public String info(String s) {
        try {
            HubFrontend frontend = new HubFrontend(_zkNaming);

            Hub.InfoStationRequest info_req = Hub.InfoStationRequest.newBuilder().setStation(s).build();

            Hub.InfoStationResponse info_res = frontend.infoStation(info_req);

            Hub.StationData station = info_res.getStation();

            double latitude = station.getLat();
            double longitude = station.getLong();

            return station.getName() + ", lat " + latitude + ", " + longitude + " long, " + station.getDocks()
                    + " docas, " + station.getPrize() + " BIC prémio, " + station.getBikes() + " bicicletas, "
                    + station.getLifts() + " levantamentos, " + station.getReturns() + " devoluções, "
                    + googleMapsLink(latitude, longitude);
        } catch (RuntimeException run_ex) {
            return run_ex.getMessage();
        }
    }

    public String bikeUp(String station) {
        try {
            HubFrontend frontend = new HubFrontend(_zkNaming);

            Hub.BikeUpRequest info_req = Hub.BikeUpRequest.newBuilder()
                    .setUsername(_name)
                    .setLat(_lat)
                    .setLong(_long)
                    .setStation(station)
                    .build();

            Hub.BikeUpResponse info_res = frontend.bikeUp(info_req);

            String response = "";

            switch (info_res.getResponse()) {
                case OK:
                    response = "OK";
                    break;
                case OUT_OF_BOUNDS:
                    response = "ERROR fora de alcance";
                    break;
                case INSUFFICIENT_BICLOINS:
                    response = "ERROR bicloins insuficientes";
                    break;
                case HAS_BIKE:
                    response = "ERRO já tem uma bicicleta";
                    break;
                case NO_BIKES:
                    response = "ERRO a estação não tem bicicletas";
                    break;
            }

            return response;
        } catch (RuntimeException run_ex) {
            return run_ex.getMessage();
        }
    }

    public String bikeDown(String station) {
        try {
            HubFrontend frontend = new HubFrontend(_zkNaming);

            Hub.BikeDownRequest info_req = Hub.BikeDownRequest.newBuilder()
                    .setUsername(_name)
                    .setLat(_lat)
                    .setLong(_long)
                    .setStation(station)
                    .build();

            Hub.BikeDownResponse info_res = frontend.bikeDown(info_req);

            String response = "";

            switch (info_res.getResponse()) {
                case OK:
                    response = "OK";
                    break;
                case OUT_OF_BOUNDS:
                    response = "ERROR fora de alcance";
                    break;
                case NO_BIKE:
                    response = "ERRO não tem uma bicicleta";
                    break;
                case NO_DOCKS:
                    response = "ERRO a estação não tem docas livres";
                    break;
            }

            return response;
        } catch (RuntimeException run_ex) {
            return run_ex.getMessage();
        }
    }

    public String ping() {
        try {
            HubFrontend hubFrontend = new HubFrontend(_zkNaming);

            Hub.PingRequest ping_req = Hub.PingRequest.newBuilder().setInputText("app").build();

            Hub.PingResponse ping_res = hubFrontend.ping(ping_req);

            return ping_res.getOutputText();
        } catch (RuntimeException run_ex) {
            return "Error pinging hub.";
        }
    }

    public String sysStatus() {
        try {
            HubFrontend hubFrontend = new HubFrontend(_zkNaming);

            Hub.StatusRequest stat_req = Hub.StatusRequest.newBuilder().build();

            Hub.StatusResponse stat_res = hubFrontend.sysStatus(stat_req);

            String[] statusList = stat_res.getStatusList().toArray(new String[0]);

            String response = "";

            for (String status: statusList) {
                response += status + " | ";
            }

            response = response.substring(0, response.length() - 3);

            return response;
        } catch (NullPointerException e) {
            return e.toString();
        }
    }
}
