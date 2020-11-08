    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);


void dump(const Matrix<int>& matr) {

    for (auto i : matr.data) {
        for (auto j : i)
            std::cout << j << ' ';

        std::cout << '\n';
    }
}

template <typename T>
struct Matrix {

    Matrix(const int sizeX, const int sizeY);
    std::vector<std::vector<T>> data;
    std::vector<T>& operator[](const int indx);
    const std::vector<T>& operator[](const int indx) const;

    const int sizeX = 0;
    const int sizeY = 0;

};

template <typename T>
Matrix<T>::Matrix(const int sizeX, const int sizeY) : sizeX(sizeX), sizeY(sizeY) {

    data.resize(sizeY);
    data.shrink_to_fit();
    for (auto& line : data) {

        line.resize(sizeX);
        line.shrink_to_fit();

    }

}

template <typename T>
std::vector<T>& Matrix<T>::operator[](const int indx) {
    return data[indx];
}

template <typename T>
const std::vector<T>& Matrix<T>::operator[](const int indx) const {
    return data[indx];
}

