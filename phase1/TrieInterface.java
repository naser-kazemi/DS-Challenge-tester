package phase1;

import java.util.List;
import java.util.Set;

public interface TrieInterface {

    void add(String str);

    boolean contains(String str);

    List<String> prefixWords(String prefix);

    Set<String> wildcardMatches(String pattern);
}
