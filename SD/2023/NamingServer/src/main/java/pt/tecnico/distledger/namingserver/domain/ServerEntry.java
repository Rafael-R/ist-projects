package pt.tecnico.distledger.namingserver.domain;

import pt.tecnico.distledger.namingserver.NamingServer;

public class ServerEntry {

    private String address;
    private String qualifier;

    public ServerEntry(String address, String qualifier) {
        this.address = address;
        this.qualifier = qualifier;

        NamingServer.debug("Created server with address '" + address +
                "' and qualifier '" + qualifier + "'.");
    }

    public String getAddress() {
        return this.address;
    }

    public String getQualifier() {
        return this.qualifier;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public void setQualifier(String qualifier) {
        this.qualifier = qualifier;
    }

    public void remove() {
        NamingServer.debug("Removed server '" + this.address + "'.");

        this.address = null;
        this.qualifier = null;
    }

    @Override
    public String toString() {
        return "{address= " + this.address + ", qualifier= " + this.qualifier + "}";
    }
}
