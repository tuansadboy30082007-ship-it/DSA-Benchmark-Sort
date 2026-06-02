#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>

using namespace std;

void gen_int(int test_num) {
    int n = 100000;
    cout << n << "\n";
    if (test_num == 1) {
        for (int i = 0; i < n; i++) cout << i << "\n";
    }
    else if (test_num == 2) {
        for (int i = 0; i < n; i++) cout << n - i << "\n";
    }
    else if (test_num == 3) {
        for (int i = 0; i < n; i++) cout << 1005 << "\n";
    }
    else if (test_num == 4) {
        for (int i = 0; i < n / 2; i++) cout << "2147483647\n-2147483648\n";
    }
    else if (test_num == 5) {
        mt19937 r(2111);
        uniform_int_distribution<long long> d(-2147483648LL, 2147483647LL);
        for (int i = 0; i < n; i++) cout << d(r) << "\n";
    }
}

void gen_strlexi(int test_num) {
    int n = 100000;
    cout << n << "\n";
    if (test_num == 1) {
        string s(100, 'a');
        for (int i = 0; i < n; i++) cout << s << "\n";
    }
    else if (test_num == 2) {
        string prefix(99, 'a');
        for (int i = 0; i < n; i++) cout << prefix << (char)('a' + (i % 26)) << "\n";
    }
    else if (test_num == 3) {
        vector<string> v(n);
        for (int i = 0; i < n; i++) v[i] = string(100, (char)('z' - (i % 26)));
        sort(v.rbegin(), v.rend());
        for (int i = 0; i < n; i++) cout << v[i] << "\n";
    }
    else if (test_num == 4) {
        vector<string> v(n);
        for (int i = 0; i < n; i++) v[i] = string(10 + (i % 91), 'a');
        sort(v.rbegin(), v.rend());
        for (int i = 0; i < n; i++) cout << v[i] << "\n";
    }
    else if (test_num == 5) {
        mt19937 r(2111);
        uniform_int_distribution<int> len_dist(10, 100);
        uniform_int_distribution<int> char_dist('a', 'z');
        for (int i = 0; i < n; i++) {
            int len = len_dist(r);
            string s(len, ' ');
            for (int j = 0; j < len; j++) s[j] = (char)char_dist(r);
            cout << s << "\n";
        }
    }
}


void gen_strlenlexi(int test_num) {
    int n = 10000;
    cout << n << "\n";
    if (test_num == 1) {
        vector<string> v(n);
        for (int i = 0; i < n; i++) v[i] = string(99, 'a') + (char)('z' - (i % 26));
        sort(v.rbegin(), v.rend());
        for (int i = 0; i < n; i++) cout << v[i] << "\n";
    }
    else if (test_num == 2) {
        string short_s(10, 'a');
        string long_s(100, 'z');
        for (int i = 0; i < n; i++) {
            if (i % 2 == 0) cout << short_s << "\n";
            else cout << long_s << "\n";
        }
    }
    else if (test_num == 3) {
        for (int i = 0; i < n; i++) {
            int len = 100 - (i % 91);
            cout << string(len, 'a') << "\n";
        }
    }
    else if (test_num == 4) {
        string s(100, 'z');
        for (int i = 0; i < n; i++) cout << s << "\n";
    }
    else if (test_num == 5) {
        mt19937 r(2111);
        uniform_int_distribution<int> len_dist(10, 100);
        uniform_int_distribution<int> char_dist('a', 'z');
        for (int i = 0; i < n; i++) {
            int len = len_dist(r);
            string s(len, ' ');
            for (int j = 0; j < len; j++) s[j] = (char)char_dist(r);
            cout << s << "\n";
        }
    }
}

int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string type = argv[1];
    int test_num = stoi(argv[2]); 
    if (type == "int") {
        gen_int(test_num);
    }
    else if (type == "strlexi") {
        gen_strlexi(test_num);
    }
    else if (type == "strlenlexi") {
        gen_strlenlexi(test_num);
    }

    return 0;
}