package pt.tecnico.bicloin.hub;


import com.opencsv.bean.CsvBindByPosition;


public class User {

    @CsvBindByPosition(position = 0)
    private String username;

    @CsvBindByPosition(position = 1)
    private String name;

    @CsvBindByPosition(position = 2)
    private String phoneNumber;

    public String getUsername() {
        return username;
    }

    public String getPhoneNumber() { return phoneNumber; }

    public String toString() {
        return username + " | " + name + " | " + phoneNumber;
    }
}
