package phase1;

import java.util.HashMap;
import java.util.Map;

public class Node {

    protected final Map<Character, Node> children;
    boolean isEnd;

    public Node() {
        children = new HashMap<>(0);
        isEnd = false;
    }
}
