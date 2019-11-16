package m19.users;

import java.util.Comparator;
import java.util.Map;

public class UserComparator implements Comparator<User> {

    public int compare(User user1, User user2) {
        return (user1.compareTo(user2));
    }
    
}