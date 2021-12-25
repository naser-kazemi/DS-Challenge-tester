#include <stdio.h>
#include <bits/stdc++.h>

using namespace std;

typedef struct Node {
    string value;
    vector<string> words;
    vector<Node*> children;
    int hashes[26]{};

    explicit Node(string v) {
        value = v;
        for (int i = 0; i < 26; i++) {
            hashes[i] = -1;
        }
    }
} Node;

class Tree {
private:
    static int my_hash(char c) {
        return int(c) - 97;
    }

    static bool hasEnding(string const &fullString, string const &ending) {
        if (fullString.length() >= ending.length()) {
            return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
        } else {
            return false;
        }
    }

public:
    Node* mainRoot = new Node("");

    Tree() = default;

    void insert(string word, Node* root) {
        if (word.length() == 0) {
            root->words.emplace_back("");
            return;
        }
        char c = word[0];
        int nodeHash = root->hashes[my_hash(c)];
        if (nodeHash == -1) {
            root->hashes[my_hash(c)] = root->children.size();
            Node* x = new Node(root->value + c);
            root->children.push_back(x);
            nodeHash = root->hashes[my_hash(c)];
        }
        root->words.push_back(word);

        Node* nextNode = root->children[nodeHash];
        insert(word.substr(1), nextNode);
    }

    string find(string word, Node* root) {
        char firstChar = word[0];
        if (firstChar == '\\') {
            word = word.substr(3);
            int count = 0;
            vector<string> result;
            for (int i = 0; i < root->words.size(); i++) {
                if (hasEnding(root->words[i], word)) {
                    result.push_back(root->value + root->words[i]);
                    count += 1;
                }
            }
            string a = accumulate(begin(result), end(result), string(),
                                  [](string &ss, string &s) {
                                      return ss.empty() ? s : ss + " " + s;
                                  });
            return to_string(count) + ' ' + a;
        }
        int nodeHash = root->hashes[my_hash(firstChar)];
        if (nodeHash == -1)
            return "0";

        return find(word.substr(1), root->children[nodeHash]);
    }
};

int main() {
    Tree tree;
    int n, q;
    scanf("%d %d\n", &n, &q);

    char temp[30];
    for (int i = 0; i < n; i++) {
        scanf("%s", temp);
        tree.insert(temp, tree.mainRoot);
    }

    for (int i = 0; i < q; i++) {
        scanf("%s", temp);
        printf("%s\n", tree.find(temp, tree.mainRoot).c_str());
    }

    return 0;
}
