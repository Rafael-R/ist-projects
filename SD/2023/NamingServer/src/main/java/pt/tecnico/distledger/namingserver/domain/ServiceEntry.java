package pt.tecnico.distledger.namingserver.domain;

import pt.tecnico.distledger.namingserver.NamingServer;
import pt.tecnico.distledger.namingserver.domain.exception.ServerAlreadyRegisteredException;
import pt.tecnico.distledger.namingserver.domain.exception.ServerNotRegisteredException;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class ServiceEntry {

    private String service;
    private Set<ServerEntry> serverEntries;

    public ServiceEntry(String service) {
        this.service = service;
        this.serverEntries = new HashSet<>();

        NamingServer.debug("Service '" + service + "' created.");
    }

    public String getService() {
        return this.service;
    }

    public Set<ServerEntry> getServerEntries() {
        return this.serverEntries;
    }

    public void setService(String service) {
        this.service = service;
    }

    public void setServerEntries(Set<ServerEntry> serverEntries) {
        this.serverEntries = serverEntries;
    }

    /**
     * Add server entry to service
     * @param serverEntry represents a server
     */
    public void addServerEntry(ServerEntry serverEntry) {
        if (this.serverEntries.stream()
                .filter(serverEntry1 -> serverEntry1.getAddress().equals(serverEntry.getAddress()))
                .count() == 0) {
            this.serverEntries.add(serverEntry);

            NamingServer.debug("Added server '" + serverEntry.getAddress() +
                    "' to service '" + this.service + "'.");
        } else { // If server is already registered
            throw new ServerAlreadyRegisteredException(serverEntry.getAddress());
        }
    }

    /**
     * Remove server entry from service
     * @param serverEntry represents a server
     */
    public void removeServerEntry(ServerEntry serverEntry) {
        NamingServer.debug("Removed server '" + serverEntry.getAddress() +
                "' from service '" + this.service + "'.");

        this.serverEntries.remove(serverEntry);
        serverEntry.remove();
    }

    /**
     * Remove server entry by its address
     * @param address server address
     */
    public void removeServerEntry(String address) {
        boolean found = false;

        List<ServerEntry> serverEntries = new ArrayList<>(this.serverEntries);
        for (int i = serverEntries.size() - 1; i >= 0; i--) {
            ServerEntry serverEntry = serverEntries.get(i);
            if(serverEntry.getAddress().equals(address)) {
                removeServerEntry(serverEntry);
                found = true;
            }
        }

        if (!found) {
            throw new ServerNotRegisteredException(address);
        }
    }

    public String toString() {
        return "{service= " + this.service + ", serverEntries= " + this.serverEntries.toString() + "}";
    }
}
