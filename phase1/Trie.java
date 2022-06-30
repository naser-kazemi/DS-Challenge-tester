package phase1;

import java.util.*;
import java.util.Map.Entry;

public class Trie implements TrieInterface {

    public static final String WILDCARD = "\\S";
    public static final String WILDCARD_STAR = "\\S*";


    private final Node root;
    private int size;
    private List<String> patterns;
    public HashMap<String, Integer> frequency = new HashMap<>();

    public Trie() {
        this.root = new Node();
        this.size = 0;
    }

    public Trie(Collection<String> col) {
        this();
        for (String s : col) {
            this.add(s);
            if (this.frequency.containsKey(s))
                this.frequency.put(s, this.frequency.get(s) + 1);
            else
                this.frequency.put(s, 1);
        }
    }

    public Trie(String[] arr) {
        this();
        for (String s : arr) {
            this.add(s);
            if (this.frequency.containsKey(s))
                this.frequency.put(s, this.frequency.get(s) + 1);
            else
                this.frequency.put(s, 1);
        }
    }


    @Override
    public void add(String str) {
        Node currentNode = this.root;
        for (int i = 0; i < str.length(); i++) {
            if (currentNode.children.get(str.charAt(i)) == null)
                currentNode.children.put(str.charAt(i), new Node());
            currentNode = currentNode.children.get(str.charAt(i));
        }
        if (!currentNode.isEnd)
            this.size++;
        currentNode.isEnd = true;
    }


    private Node getNode(String str) {
        Node node = this.root;
        for (int i = 0; i < str.length(); i++) {
            Node child = node.children.get(str.charAt(i));
            if (child == null)
                return null;
            node = child;
        }
        return node;
    }


    @Override
    public boolean contains(String str) {
        Node node = getNode(str);
        return node != null && node.isEnd;
    }

    private static void DFS(Node root, String prefix, List<String> list) {
        if (root == null)
            return;
        if (root.isEnd)
            list.add(prefix);
        for (Entry<Character, Node> entry : root.children.entrySet())
            DFS(entry.getValue(), prefix + entry.getKey(), list);
    }


    @Override
    public List<String> prefixWords(String prefix) {
        Node current = getNode(prefix);
        List<String> prefixWords = new ArrayList<>();
        DFS(current, prefix, prefixWords);
        return prefixWords;
    }

    private void wildcardTraverse(String pattern, StringBuilder prefix, Node root, int len, Set<String> wildcardMatches) {
        if (root == null) return;
        if (len == pattern.length()) {
            if (root.isEnd)
                wildcardMatches.add(prefix.toString());
            return;
        }
        if (pattern.length() >= 3 + len && pattern.substring(len, len + 3).equals(WILDCARD_STAR)) {
            wildcardTraverse(pattern, prefix, root, len + 3, wildcardMatches);
            for (Entry<Character, Node> entry : root.children.entrySet()) {
                prefix.append(entry.getKey());
                wildcardTraverse(pattern, prefix, entry.getValue(), len, wildcardMatches);
                prefix.deleteCharAt(prefix.length() - 1);
            }
        } else if (pattern.length() >= 2 + len && pattern.substring(len, len + 2).equals(WILDCARD)) {
            for (Entry<Character, Node> entry : root.children.entrySet()) {
                prefix.append(entry.getKey());
                wildcardTraverse(pattern, prefix, entry.getValue(), len + 2, wildcardMatches);
                prefix.deleteCharAt(prefix.length() - 1);
            }
        } else {
            prefix.append(pattern.charAt(len));
            wildcardTraverse(pattern, prefix, root.children.get(pattern.charAt(len)), len + 1, wildcardMatches);
            prefix.deleteCharAt(prefix.length() - 1);
        }
    }


    @Override
    public Set<String> wildcardMatches(String pattern) {
        Set<String> wildcardMatches = new HashSet<>();
        wildcardTraverse(pattern, new StringBuilder(), root, 0, wildcardMatches);
        return wildcardMatches;
    }

    public List<String> getPatterns() {
        return patterns;
    }

    public void setPatterns(List<String> patterns) {
        this.patterns = patterns;
    }


    public void respondQueries() {
        for (String pattern : this.patterns) {
            Set<String> matches = this.wildcardMatches(pattern);
            ArrayList<String> allMatches = new ArrayList<>();
            for (String match : matches)
                for (int i = 0; i < this.frequency.get(match); i++)
                    allMatches.add(match);
            String result = allMatches.toString().replaceAll(",", "").replace("[", "")
                    .replace("]", "");
            System.out.println(allMatches.size() + " " + result);
        }
    }


}
