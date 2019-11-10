package m19.users;

import java.util.Comparator;
import java.util.Map;

public class UserComparator implements Comparator<Map.Entry<Integer, User>> {

    public int compare(Map.Entry<Integer, User> e1, Map.Entry<Integer, User> e2) {
        return (e1.getValue()).compareTo(e2.getValue());
    }
    
}