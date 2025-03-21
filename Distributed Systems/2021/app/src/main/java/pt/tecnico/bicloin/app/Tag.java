package pt.tecnico.bicloin.app;

public class Tag {
    private final String _name;
    private final double _lat;
    private final double _long;


    public Tag(double coordinatesX, double coordinatesY, String name) {
        _name = name;
        _lat = coordinatesX;
        _long = coordinatesY;
    }

    public String getName() {
        return _name;
    }

    public double getLatitude() {
        return _lat;
    }

    public double getLongitude() {
        return _long;
    }
}
