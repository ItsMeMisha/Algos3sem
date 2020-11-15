#include <iostream>
#include <vector>
#include <string>

std::vector<int> Z_func(const std::vector<int>& n) {
    auto ans = std::vector<int>(n.size(), 0);
    ans[0] = n.size();

    int l = 0;
    int r = 0;

    for (int i = 1; i < n.size(); ++i) {
        ans[i] = std::max(0, std::min(r - i, ans[i - l]));

        while (i + ans[i] < n.size() && n[ans[i]] == n[i + ans[i]]) {
            ++ans[i];
        }

        if (i + ans[i] > r) {
            l = i;
            r = i + ans[i];
        }
    }

    return ans;
}

int main(void) {
    size_t N = 0;
    size_t C = 0;

    std::cin >> N >> C;

    auto n = std::vector<int>(2 * N + 1, -1);

    for (size_t i = 0; i < N; ++i) {
        std::cin >> n[i];
        n[2 * N - i] = n[i];
    }

    auto z = Z_func(n);

    for (size_t i = N + 1; i < n.size(); ++i) {
        if (z[i] == n.size() - i && z[i] % 2 == 0) {
            std::cout << N - z[i]/2 << " ";
        }
    }

    std::cout << N;
}
