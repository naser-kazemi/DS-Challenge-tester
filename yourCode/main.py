from typing import Optional


class TrieNode:
    def __init__(self):
        self.children: list[Optional[TrieNode]] = [None] * 26
        self.is_end_of_word: int = 0


class Trie:
    def __init__(self):
        self.root: TrieNode = TrieNode()

    @staticmethod
    def _letter_to_index(letter: str) -> int:
        """Return ascii code of lowered letter mapped to 0 to 26

        Return -1 if letter is not a-z or A-Z
        """
        letter_ascii: int = ord(letter)
        if 65 <= letter_ascii <= 90:
            return letter_ascii - 65
        elif 97 <= letter_ascii <= 122:
            return letter_ascii - 97
        else:
            return -1

    @staticmethod
    def _index_to_letter(n: int) -> str:
        """return (n + 1)th letter in alphabet"""
        return chr(n + 97)

    def insert(self, word: str):
        """Insert word into trie"""
        current_node = self.root
        for letter in word:
            index = Trie._letter_to_index(letter)
            if index == -1:
                continue
            if current_node.children[index] is None:
                current_node.children[index] = TrieNode()
            current_node = current_node.children[index]
        current_node.is_end_of_word += 1

    def search_prefix(self, prefix: str) -> list[str]:
        """Return words start with the prefix in the trie"""
        current_node = self.root
        for letter in prefix:
            index = Trie._letter_to_index(letter)
            if current_node.children[index] is None:
                return []
            current_node = current_node.children[index]
        matched_words = Trie.traverse_subtree(current_node, prefix)
        return matched_words

    @staticmethod
    def traverse_subtree(subtree_root: TrieNode, root_value: str) -> list[str]:
        """Return all words in subtree"""
        words = []
        for i in range(subtree_root.is_end_of_word):
            words.append(root_value)
        for i, child in enumerate(subtree_root.children):
            if child is not None:
                words.extend(Trie.traverse_subtree(child, root_value + Trie._index_to_letter(i)))
        return words


def main():
    words_count, patterns_count = map(int, input().split())
    words = input().split()
    patterns = []
    for i in range(patterns_count):
        patterns.append(input().lower())

    result = match_patterns(words, patterns)
    for matched_words in result:
        print(len(matched_words), end=' ')
        print(' '.join(matched_words))


def match_patterns(words: list[str], patterns: list[str]) -> list[list[str]]:
    """Return a list of lists containing matched words for each pattern"""
    trie = Trie()
    reversed_trie = Trie()
    for word in words:
        trie.insert(word)
        reversed_trie.insert(word[::-1])

    result = []
    for pattern in patterns:
        prefix, suffix = pattern.split("\\s*")
        if len(prefix) == 0 and len(suffix) == 0:
            matched_words = words
        elif len(prefix) == 0:
            suffix_matched_words = reversed_trie.search_prefix(suffix[::-1])
            matched_words = []
            for word in suffix_matched_words:
                matched_words.append(word[::-1])
        elif len(suffix) == 0:
            matched_words = trie.search_prefix(prefix)
        else:
            prefix_matched_words = trie.search_prefix(prefix)
            suffix_matched_words = reversed_trie.search_prefix(suffix[::-1])

            pattern_len = len(prefix) + len(suffix)
            matched_words = []
            for word in prefix_matched_words:
                if len(word) >= pattern_len and word[::-1] in suffix_matched_words:
                    matched_words.append(word)
        result.append(matched_words)
    return result


if __name__ == '__main__':
    main()
