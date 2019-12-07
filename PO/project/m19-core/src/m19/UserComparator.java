package m19;

import java.util.Comparator;

public class UserComparator implements Comparator<User> {

    @Override
    public int compare(User user1, User user2) {
        return (user1.compareTo(user2));
    }
    
}