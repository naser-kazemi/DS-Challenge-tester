package phase1;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Main {

    static class InputObject {
        public int wordsNumber;
        public int queryNumber;
        public String[] words;
        List<String> patterns = new ArrayList<>();
    }


    private static InputObject getInputObject(Scanner scanner) {
        InputObject inputObject = new InputObject();
        inputObject.wordsNumber = scanner.nextInt();
        inputObject.queryNumber = scanner.nextInt();
        scanner.nextLine();
        inputObject.words = scanner.nextLine().toLowerCase().split(" ");
        for (int i = 0; i < inputObject.queryNumber; i++)
            inputObject.patterns.add(scanner.nextLine());
        return inputObject;
    }

    private static InputObject getInputFromFile() throws FileNotFoundException {
        File input = new File("src/trie/input.txt");
        Scanner scanner = new Scanner(input);
        return getInputObject(scanner);
    }

    private static InputObject getInputFromConsole() {
        Scanner scanner = new Scanner(System.in);
        return getInputObject(scanner);
    }


//    public static void main(String[] args) throws Exception {
//        InputObject inputObject = getInputFromFile();
//        Trie trie = new Trie(inputObject.words);
//        trie.setPatterns(inputObject.patterns);
//        trie.respondQueries();
//    }

    public static void main(String[] args) {
        InputObject inputObject = getInputFromConsole();
        Trie trie = new Trie(inputObject.words);
        trie.setPatterns(inputObject.patterns);
        trie.respondQueries();
    }

}