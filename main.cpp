#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <sstream>
#include <regex>
#include <memory>
#include <chrono>

using namespace std;

template<typename Key_, typename Value_>
struct HAItem {
    HAItem *Next;
    Key_ Key;
    Value_ Value;
    size_t Hash;
};


#ifndef HARRAY_DOUBLE_SIZE_HASH_TABLE_
#define HARRAY_DOUBLE_SIZE_HASH_TABLE_ 1
#endif

template<typename Key_, typename Value_, typename Hash_ = std::hash<Key_>,
        typename Allocator_ = std::allocator<HAItem<Key_, Value_>>>


class HArray {

    using HAItem_T = HAItem<Key_, Value_>;

public:
    HArray() = default;

    ~HArray() {
        delete[] hash_table_;
        destruct_range_(storage_, (storage_ + index_));
        allocator_.deallocate(storage_, capacity_);
    }

    HArray(HArray &&h_arr) noexcept
            : capacity_(h_arr.capacity_), base_(h_arr.base_), index_(h_arr.index_),
              size_(h_arr.size_), hash_table_(h_arr.hash_table_),
              storage_(h_arr.storage_), allocator_(h_arr.allocator_) {
        h_arr.capacity_ = 0;
        h_arr.base_ = 0;
        h_arr.index_ = 0;
        h_arr.size_ = 0;
        h_arr.hash_table_ = nullptr;
        h_arr.storage_ = nullptr;
    }

    HArray &operator=(HArray &&h_arr) noexcept {
        if (this != &h_arr) {
            delete[] hash_table_;
            destruct_range_(storage_, (storage_ + index_));
            allocator_.deallocate(storage_, capacity_);

            capacity_ = h_arr.capacity_;
            base_ = h_arr.base_;
            index_ = h_arr.index_;
            size_ = h_arr.size_;

            h_arr.capacity_ = 0;
            h_arr.base_ = 0;
            h_arr.index_ = 0;
            h_arr.size_ = 0;

            hash_table_ = h_arr.hash_table_;
            storage_ = h_arr.storage_;
            allocator_ = h_arr.allocator_;
            h_arr.hash_table_ = nullptr;
            h_arr.storage_ = nullptr;
        }

        return *this;
    }

    void operator+=(HArray &&h_arr) {
        const size_t n_index = (index_ + h_arr.index_);

        if (n_index > capacity_) {
            delete[] hash_table_;
            set_base_(n_index);
            allocator_hash_table_();
            resize_(n_index);
        }

        HAItem_T *src_item = h_arr.storage_;
        const HAItem_T *end = (h_arr.storage_ + h_arr.index_);

        while (src_item != end) {
            if (src_item->Hash != 0) {
                HAItem_T **item = find_(src_item->Key, src_item->Hash);

                if ((*item) == nullptr) {
                    (*item) = (storage_ + index_);
                    insert_(static_cast<Key_ &&>(src_item->Key),
                            src_item->Hash);
                } else {
                    src_item->Key.~Key_();
                }

                (*item)->Value = static_cast<Value_ &&>(src_item->Value);
            }

            ++src_item;
        }

        delete[] h_arr.hash_table_;
        h_arr.hash_table_ = nullptr;
        allocator_.deallocate(h_arr.storage_, h_arr.capacity_);
        h_arr.storage_ = nullptr;

        h_arr.capacity_ = 0;
        h_arr.base_ = 0;
        h_arr.index_ = 0;
        h_arr.size_ = 0;
    }

    void operator+=(const HArray &h_arr_c) {
        const size_t n_index = (index_ + h_arr_c.index_);

        if (n_index > capacity_) {
            delete[] hash_table_;
            set_base_(n_index);
            allocator_hash_table_();
            resize_(n_index);
        }

        HAItem_T *c_item = h_arr_c.storage_;
        const HAItem_T *end = (h_arr_c.storage_ + h_arr_c.index_);

        while (c_item != end) {
            if (c_item->Hash != 0) {
                HAItem_T **item = find_(c_item->Key, c_item->Hash);

                if ((*item) == nullptr) {
                    (*item) = (storage_ + index_);
                    insert_(Key_(c_item->Key), c_item->Hash);
                }

                (*item)->Value = Value_(c_item->Value);
            }

            ++c_item;
        }
    }

    Value_ &operator[](Key_ &&key) {
        const size_t hash = hash_fun_(key);

        if (capacity_ == 0) {
            init_storage_();
        }

        HAItem_T **item = find_(key, hash);

        if ((*item) == nullptr) {
            if (index_ == capacity_) {
                grow_();
                item = find_(key, hash);
            }

            (*item) = (storage_ + index_);
            insert_(static_cast<Key_ &&>(key), hash);
        }

        return (*item)->Value;
    }

    inline Value_ &operator[](const Key_ &key) {
        return (*this)[Key_(key)];
    }


    const HAItem_T *GetItem(size_t index) const {
        if (index < index_) {
            const HAItem_T *item = (storage_ + index);

            if (item->Hash != 0) {
                return item;
            }
        }

        return nullptr;
    }

    Value_ *Find(const Key_ &key) {
        if (capacity_ != 0) {
            HAItem_T *item = *(find_(key, hash_fun_(key)));

            if (item != nullptr) {
                return &(item->Value);
            }
        }

        return nullptr;
    }


    void Clear() {
        delete[] hash_table_;

        destruct_range_(storage_, (storage_ + index_));
        allocator_.deallocate(storage_, capacity_);

        hash_table_ = nullptr;
        storage_ = nullptr;
        capacity_ = 0;
        base_ = 0;
        index_ = 0;
        size_ = 0;
    }

    // Remove excess storage.
    void Compress() {
        if (size_ == 0) {
            Clear();
        } else if (size_ != capacity_) {
            const size_t old_base = base_;
            set_base_(size_);

            if (old_base != base_) {
                delete[] hash_table_;
                allocator_hash_table_();
            } else {
                /*
                 * Clear the hash table if there is no need to reallocate
                 * it.
                 */
                HAItem_P *item_ht = hash_table_;
                const HAItem_P *end_ht = (hash_table_ + old_base + 1);

                while (item_ht != end_ht) {
                    item_ht->Anchor = nullptr;
                    ++item_ht;
                }
            }

            resize_(size_);
        }
    }

    [[nodiscard]] inline size_t ArraySize() const noexcept {
        return index_;
    }


    //////////// Private ////////////

private:
    struct HAItem_P {
        HAItem_T *Anchor{nullptr};
    };

    size_t hash_fun_(const Key_ &key) const noexcept {
        static const Hash_ hash{};
        const size_t hash_value = hash(key);

        return ((hash_value != 0) ? hash_value : 1);
    }

    void allocator_hash_table_() {
        hash_table_ = new HAItem_P[(base_ + 1)];
    }

    void destruct_range_(HAItem_T *start, const HAItem_T *end) {
        while (start != end) {
            start->~HAItem_T();
            ++start;
        }
    }

    void grow_() {
        ++base_;
        base_ *= 2;
        --base_;

        delete[] hash_table_;
        allocator_hash_table_();
        resize_(capacity_ * 2);
    }


    void init_storage_() {
#if HARRAY_DOUBLE_SIZE_HASH_TABLE_ == 1
        base_ = 1;
#else
        base_ = 0;
#endif
        capacity_ = 1;
        allocator_hash_table_();
        storage_ = allocator_.allocate(capacity_);
    }

#ifdef _MSC_VER
    inline static unsigned long CLZL(unsigned long value) {
        unsigned long index = 0;

        if (_BitScanReverse(&index, value)) {
            return index;
        }

        return 0;
    }
#else

    inline static unsigned int CLZL(unsigned long value) {
        constexpr unsigned int bits = (sizeof(long) * 8) - 1;

        if (value != 0) {
            return (bits - static_cast<unsigned int>(__builtin_clzl(value)));
        }

        return 0;
    }

#endif

    void set_base_(size_t n_size) noexcept {
        base_ = 1U;
        base_ <<= CLZL(static_cast<unsigned long>(n_size));

        if (base_ < n_size) {
            base_ <<= 1U;
        }

#if HARRAY_DOUBLE_SIZE_HASH_TABLE_ == 1
        base_ <<= 1U;
#endif
        --base_;
    }

    HAItem_T **find_(const Key_ &key, size_t hash) {
        HAItem_T **item = &((hash_table_ + (hash & base_))->Anchor);

        while (((*item) != nullptr) && ((*item)->Key != key)) {
            item = &((*item)->Next);
        }

        return item;
    }

    inline void insert_(Key_ &&key, size_t hash) {
        new(storage_ + index_)
                HAItem_T{nullptr, static_cast<Key_ &&>(key), Value_(), hash};
        ++index_;
        ++size_;
    }

    void resize_(size_t new_size) {
        HAItem_T *src = storage_;
        storage_ = allocator_.allocate(new_size);

        HAItem_T *des_item = storage_;
        HAItem_T *src_item = src;
        const HAItem_T *end = (src + index_);

        while (src_item != end) {
            if (src_item->Hash != 0) {
                new(des_item) HAItem_T{
                        nullptr, static_cast<Key_ &&>(src_item->Key),
                        static_cast<Value_ &&>(src_item->Value), src_item->Hash};

                ++des_item;
            }

            ++src_item;
        }

        allocator_.deallocate(src, capacity_);

        index_ = static_cast<size_t>(des_item - storage_);
        size_ = index_;
        capacity_ = new_size;

        generate_hash_();
    }

    void generate_hash_() {
        // hash_table_ should be null before calling this function.
        const HAItem_T *end = (storage_ + size_);
        HAItem_T *item = storage_;

        while (item != end) {
            HAItem_T **position =
                    &((hash_table_ + (item->Hash & base_))->Anchor);

            while ((*position) != nullptr) {
                position = &((*position)->Next);
            }

            (*position) = item;
            ++item;
        }
    }

    size_t capacity_{0};
    size_t base_{0};
    size_t index_{0};
    size_t size_{0};
    HAItem_P *hash_table_{nullptr};
    HAItem_T *storage_{nullptr};
    Allocator_ allocator_{};

};


class Node {

public:
//    unordered_map<char, Node> children;
    Node *children[26];
    vector<char> children_vec;
    bool is_end{};


};


string WILDCARD = "\\S";
string WILDCARD_STAR = "\\S*";


class Trie {


public:

    Node *root;
    size_t size;
    vector<string> patterns;
    HArray<string, int> freq;
    HArray<string, vector<pair<int, int>>>
            docs;

    Trie();

    explicit Trie(const vector<string> &vec);

    Trie(int n, const vector<string> *files);

    void add(const string &s);

    void wildcard_traverse(string pattern, string prefix, Node *root, int len, set<string> *wildcard_matches);

    set<string> wildcard_matches(string pattern);

    double respond_queries();

    void set_patterns(vector<string> patterns);
};

Trie::Trie() {
    Trie::root = new Node();
    Trie::size = 0;
}

Trie::Trie(const vector<string> &vec) {
    Trie::root = new Node();
    Trie::size = 0;
    for (const auto &s: vec) {
        Trie::add(s);

        if (Trie::freq.Find(s) != nullptr)
            Trie::freq[s] = Trie::freq[s] + 1;
        else
            Trie::freq[s] = 1;
    }
}


using my_item_T = HAItem<std::string, int>;

Trie::Trie(int n, const vector<string> files[]) {
    Trie::root = new Node();
    Trie::size = 0;
    for (int i = 0; i < n; i++) {
        HArray<string, int> frequency;
        for (const auto &s: files[i]) {
            Trie::add(s);
            if (frequency.Find(s) != nullptr)
                frequency[s] = frequency[s] + 1;
            else
                frequency[s] = 1;
        }
        for (size_t j = 0; j < frequency.ArraySize(); j++) {
            const my_item_T *item = frequency.GetItem(j);
            if (item) {
                if (docs.Find(item->Key) == nullptr) {
                    docs[item->Key] = vector<pair<int, int >>();
                    docs[item->Key].emplace_back(i + 1, item->Value);
                } else
                    docs[item->Key].emplace_back(i + 1, item->Value);
            }
        }
    }
    Trie::docs.Compress();
}


void Trie::add(const string &s) {
    Node *current_node = Trie::root;
    for (char i: s) {
//        if (current_node->children.find(i) == current_node->children.end())
//            current_node->children[i] = Node();
        if (current_node->children[i - 'a'] == nullptr) {
            current_node->children[i - 'a'] = new Node();
            current_node->children_vec.push_back(i);
        }
        current_node = current_node->children[i - 'a'];
    }
    if (!current_node->is_end)
        Trie::size++;
    current_node->is_end = true;
}


void Trie::wildcard_traverse(string pattern, string prefix, Node *root, int len, set<string> *wildcard_matches) {
    if (root == nullptr)
        return;
    if (len == pattern.length()) {
        if (root->is_end)
            wildcard_matches->insert(prefix);
        return;
    }
    if (pattern.length() >= 3 + len && pattern.substr(len, 3) == WILDCARD_STAR) {
        wildcard_traverse(pattern, prefix, root, len + 3, wildcard_matches);
        for (auto i: root->children_vec) {
            prefix.push_back((char) (i));
            wildcard_traverse(pattern, prefix, root->children[i - 'a'], len, wildcard_matches);
            prefix.pop_back();
        }

    } else {
        prefix.push_back(pattern[len]);
        wildcard_traverse(pattern, prefix, root->children[pattern[len] - 'a'], len + 1, wildcard_matches);
        prefix.pop_back();
    }
}


set<string> Trie::wildcard_matches(string pattern) {
    auto *wildcardMatches = new set<string>;
    wildcard_traverse(std::move(pattern), "", root, 0, wildcardMatches);
    return *wildcardMatches;
}

bool sortbyfirst(const pair<int, int> &a,
                 const pair<int, int> &b) {
    return (a.first > b.first);
}

double Trie::respond_queries() {
    double time = 0.0;
    ofstream fout("result.txt");
    for (int k = 0; k < Trie::patterns.size(); k++) {
        long start_time = chrono::duration_cast<std::chrono::nanoseconds>(
                chrono::system_clock::now().time_since_epoch()).count();
        set<string> matches = wildcard_matches(Trie::patterns[k]);
        int ans[10] = {0};
        for (const auto &s: matches)
            for (auto i: docs[s])
                ans[i.first - 1] += i.second;
        vector<pair<int, int>> q;
        for (int i = 0; i < 10; i++)
            if (ans[i] > 0)
                q.emplace_back(ans[i], i + 1);
        stable_sort(q.begin(), q.end(), sortbyfirst);

        long end_time = chrono::duration_cast<std::chrono::nanoseconds>(
                chrono::system_clock::now().time_since_epoch()).count();
        time += (double) (end_time - start_time) / 1000000.0;

        if (matches.empty()) {
            fout << "-1" << endl;
            continue;
        }
        for (int i = 0; i < q.size() - 1; i++)
            fout << q[i].second << " ";
        fout << q[q.size() - 1].second << endl;
    }
    fout.close();
    return time;
}


void Trie::set_patterns(vector<string> pattern) {
    Trie::patterns = std::move(pattern);
}


Trie *trie;
vector<string> patterns;

void get_input() {
    string input;
    string files[10];
    vector<string> split[10];
    for (int i = 1; i <= 10; i++) {
        ifstream fin;
        string line;
        if (i < 10)
            fin.open("doc0" + to_string(i) + ".txt");
        else
            fin.open("doc" + to_string(i) + ".txt");
        while (getline(fin, line))
            files[i - 1] += line;
        files[i - 1] = regex_replace(files[i - 1], regex(R"([^a-zA-Z ])"), "");
        files[i - 1] = regex_replace(files[i - 1], regex(R"(  )"), " ");
        std::transform(files[i - 1].begin(), files[i - 1].end(), files[i - 1].begin(),
                       [](unsigned char c) { return std::tolower(c); });
        files[i - 1].erase(files[i - 1].find_last_not_of(" \n\r\t") + 1);
        string T;
        stringstream X(files[i - 1]);
        while (getline(X, T, ' '))
            split[i - 1].push_back(T);
        fin.close();
    }
    trie = new Trie(10, split);

    ifstream fin;
    fin.open("input.txt");
    string line;
    getline(fin, line);
    while (getline(fin, line)) {
        patterns.push_back(line);
    }
    fin.close();
    trie->set_patterns(patterns);
}

int main() {
    get_input();
    double time = trie->respond_queries();
    ofstream fout("time.txt");
    fout << time << endl;
    fout.close();
}
