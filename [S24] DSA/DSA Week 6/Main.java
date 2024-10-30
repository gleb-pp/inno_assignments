#include <iostream>
#include <vector>
#include <algorithm>

template <typename T>
struct Item {
    int ind;
    T minn;
    T maxx;

    Item(int i, T mn, T mx) : ind(i), minn(mn), maxx(mx) {}

    int getInd() const {
        return ind;
    }

    T getMinn() const {
        return minn;
    }

    T getMaxx() const {
        return maxx;
    }
};

template <typename T>
void radixSort(std::vector<Item<T>>& arr, int n) {
    for (int sign = 10; sign <= 1000000; sign *= 10) {
        bool changed = false;
        std::vector<std::vector<Item<T>>> count(10);

        for (int i = 0; i < n; i++) {
            int digit = (arr[i].getMaxx() % sign) / (sign / 10);
            if (digit != 0) {
                changed = true;
            }
            count[digit].push_back(arr[i]);
        }

        if (!changed) {
            return;
        }

        int k = 0;
        for (int j = 0; j < 10; j++) {
            for (const auto& item : count[j]) {
                arr[k++] = item;
            }
        }
    }
}

template <typename T>
void countingSort(std::vector<Item<T>>& arr, int n) {
    std::vector<std::vector<Item<T>>> count(101);

    for (int i = 0; i < n; i++) {
        count[arr[i].getMinn()].push_back(arr[i]);
    }

    int k = 0;
    for (int j = 100; j >= 0; j--) {
        for (const auto& item : count[j]) {
            arr[k++] = item;
        }
    }
}

int main() {
    int n;
    std::cin >> n;
    std::vector<Item<int>> arr;

    for (int i = 0; i < n; i++) {
        int minn, maxx;
        std::cin >> minn >> maxx;
        arr.emplace_back(i + 1, minn, maxx);
    }

    radixSort(arr, n);
    countingSort(arr, n);

    for (int i = 0; i < n; i++) {
        std::cout << arr[i].getInd() << " ";
    }

    return 0;
}
