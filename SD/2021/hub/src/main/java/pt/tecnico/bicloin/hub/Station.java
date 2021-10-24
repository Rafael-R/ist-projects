package pt.tecnico.bicloin.hub;


import com.opencsv.bean.CsvBindByPosition;


public class Station {

    @CsvBindByPosition(position = 0)
    private String name;

    @CsvBindByPosition(position = 1)
    private String abbreviation;

    @CsvBindByPosition(position = 2)
    private double latitude;

    @CsvBindByPosition(position = 3)
    private double longitude;

    @CsvBindByPosition(position = 4)
    private int docks;

    @CsvBindByPosition(position = 5)
    private int bikes;

    @CsvBindByPosition(position = 6)
    private int prize;

    public String getName() {
        return name;
    }

    public String getAbbreviation() {
        return abbreviation;
    }

    public double getLatitude() {
        return latitude;
    }

    public double getLongitude() {
        return longitude;
    }

    public int getDocks() {
        return docks;
    }

    public int getBikes() { return bikes; }

    public int getPrize() { return prize; }

    public String toString() {
        return name + " | " + abbreviation + " | " + latitude + ","  + longitude +
                " | " + docks + " | " + bikes + " | " + prize;
    }
}
