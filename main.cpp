#include <iostream>
#include <iomanip>

template <typename T>
class MatrizBase {
protected:
    int _filas;
    int _columnas;
public:
    MatrizBase(int f, int c) : _filas(f), _columnas(c) {}
    virtual ~MatrizBase() {}
    int filas() const { return _filas; }
    int columnas() const { return _columnas; }
    virtual void cargarValores() = 0;
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
    virtual void imprimir() const = 0;
    virtual T obtener(int i, int j) const = 0;
};

// aqui utilizo Matrices con memoria dinamica
template <typename T>
class MatrizDinamica : public MatrizBase<T> {
    T** _datos;
    void reservar() {
        if (this->_filas < 1) this->_filas = 1;
        if (this->_columnas < 1) this->_columnas = 1;
        _datos = new T*[this->_filas];
        for (int i = 0; i < this->_filas; ++i) {
            _datos[i] = new T[this->_columnas];
            for (int j = 0; j < this->_columnas; ++j) _datos[i][j] = T(0);
        }
    }
    void liberar() {
        if (_datos) {
            for (int i = 0; i < this->_filas; ++i) delete[] _datos[i];
            delete[] _datos;
            _datos = nullptr;
        }
    }
    void copiarDesde(const MatrizDinamica& o) {
        this->_filas = o._filas;
        this->_columnas = o._columnas;
        reservar();
        for (int i = 0; i < this->_filas; ++i)
            for (int j = 0; j < this->_columnas; ++j)
                _datos[i][j] = o._datos[i][j];
    }
public:
    MatrizDinamica(int f, int c) : MatrizBase<T>(f, c), _datos(nullptr) { reservar(); }
    ~MatrizDinamica() { liberar(); }
    MatrizDinamica(const MatrizDinamica& o) : MatrizBase<T>(o._filas, o._columnas), _datos(nullptr) { copiarDesde(o); }
    MatrizDinamica& operator=(const MatrizDinamica& o) {
        if (this != &o) { liberar(); copiarDesde(o); }
        return *this;
    }
    MatrizDinamica(MatrizDinamica&& o) noexcept : MatrizBase<T>(o._filas, o._columnas), _datos(o._datos) { o._datos = nullptr; }
    MatrizDinamica& operator=(MatrizDinamica&& o) noexcept {
        if (this != &o) {
            liberar();
            this->_filas = o._filas;
            this->_columnas = o._columnas;
            _datos = o._datos;
            o._datos = nullptr;
        }
        return *this;
    }
    void set(int i, int j, T v) { _datos[i][j] = v; }
    T obtener(int i, int j) const override { return _datos[i][j]; }
    void cargarValores() override {
        for (int i = 0; i < this->_filas; ++i)
            for (int j = 0; j < this->_columnas; ++j) {
                T v; std::cin >> v; _datos[i][j] = v;
            }
    }
    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        if (this->_filas != otra.filas() || this->_columnas != otra.columnas()) return nullptr;
        MatrizDinamica<T>* r = new MatrizDinamica<T>(this->_filas, this->_columnas);
        for (int i = 0; i < this->_filas; ++i)
            for (int j = 0; j < this->_columnas; ++j)
                r->set(i, j, this->obtener(i, j) + otra.obtener(i, j));
        return r;
    }
    void imprimir() const override {
        for (int i = 0; i < this->_filas; ++i) {
            std::cout << "| ";
            for (int j = 0; j < this->_columnas; ++j) {
                std::cout << std::fixed << std::setprecision(1) << this->obtener(i, j);
                if (j + 1 < this->_columnas) std::cout << " | ";
                else std::cout << " ";
            }
            std::cout << "|\n";
        }
    }
};

// aqui profe estan las Matrices con memoria estatica 
template <typename T, int M, int N>
class MatrizEstatica : public MatrizBase<T> {
    T _datos[M][N];
public:
    MatrizEstatica() : MatrizBase<T>(M, N) {
        for (int i = 0; i < M; ++i) for (int j = 0; j < N; ++j) _datos[i][j] = T(0);
    }
    void set(int i, int j, T v) { _datos[i][j] = v; }
    T obtener(int i, int j) const override { return _datos[i][j]; }
    void cargarValores() override {
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < N; ++j) {
                T v; std::cin >> v; _datos[i][j] = v;
            }
    }
    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        if (this->_filas != otra.filas() || this->_columnas != otra.columnas()) return nullptr;
        MatrizEstatica<T, M, N>* r = new MatrizEstatica<T, M, N>();
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < N; ++j)
                r->set(i, j, this->obtener(i, j) + otra.obtener(i, j));
        return r;
    }
    void imprimir() const override {
        for (int i = 0; i < M; ++i) {
            std::cout << "| ";
            for (int j = 0; j < N; ++j) {
                std::cout << std::fixed << std::setprecision(1) << _datos[i][j];
                if (j + 1 < N) std::cout << " | ";
                else std::cout << " ";
            }
            std::cout << "|\n";
        }
    }
};

template <typename T>
MatrizBase<T>* operator+(const MatrizBase<T>& a, const MatrizBase<T>& b) {
    return a.sumar(b);
}

int main() {
    std::cout << "--- Sistema generico de Algebra Lineal ---\n\n";
    std::cout << ">> Demostracion de Genericidad (Tipo FLOAT) <<\n\n";
    std::cout << "Creando Matriz Dinamica A (3x2)...\n";
    MatrizDinamica<float> A(3, 2);
    A.set(0,0,1.5f); A.set(0,1,2.0f);
    A.set(1,0,0.0f); A.set(1,1,1.0f);
    A.set(2,0,4.5f); A.set(2,1,3.0f);
    std::cout << "A =\n"; A.imprimir(); std::cout << "\n";
    std::cout << "Creando Matriz Estatica B (3x2)...\n";
    MatrizEstatica<float,3,2> B;
    B.set(0,0,0.5f); B.set(0,1,1.0f);
    B.set(1,0,2.0f); B.set(1,1,3.0f);
    B.set(2,0,1.0f); B.set(2,1,1.0f);
    std::cout << "B =\n"; B.imprimir(); std::cout << "\n";
    MatrizBase<float>* pA = &A;
    MatrizBase<float>* pB = &B;
    std::cout << "SUMANDO: Matriz C = A + B ...\n";
    MatrizBase<float>* C = (*pA) + (*pB);
    if (!C) { std::cout << "Dimensiones incompatibles\n"; return 0; }
    std::cout << "\nMatriz Resultado C (3x2, Tipo FLOAT):\n";
    C->imprimir();
    std::cout << "\n>> Demostracion de Limpieza de Memoria <<\n";
    std::cout << "Liberando C...\n";
    delete C;
    std::cout << "\n>> Demostracion de Genericidad (Tipo INT) <<\n\n";
    MatrizDinamica<int> X(2,2);
    X.set(0,0,1); X.set(0,1,2);
    X.set(1,0,3); X.set(1,1,4);
    MatrizEstatica<int,2,2> Y;
    Y.set(0,0,5); Y.set(0,1,6);
    Y.set(1,0,7); Y.set(1,1,8);
    MatrizBase<int>* pX = &X;
    MatrizBase<int>* pY = &Y;
    MatrizBase<int>* Z = (*pX) + (*pY);
    if (!Z) { std::cout << "Dimensiones incompatibles\n"; return 0; }
    std::cout << "Z =\n";
    Z->imprimir();
    delete Z;
    std::cout << "Sistema cerrado.\n";
    return 0;
}
