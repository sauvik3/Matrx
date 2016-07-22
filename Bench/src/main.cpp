#include <vector>
#include <functional>
#include <string>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <random>

#include "matrix.h"
#include "matrix_exception.h"

#define BEN_ORDER 8

/*---------------------------- Benchmark ------------------------------*/
class Timer {
public:
	Timer() : beg_(clock_::now()) {}
	void reset() { beg_ = clock_::now(); }
	double elapsed() const {
		return std::chrono::duration_cast<second_>
			(clock_::now() - beg_).count();
	}

private:
	typedef std::chrono::high_resolution_clock clock_;
	typedef std::chrono::duration<double, std::ratio<1> > second_;
	std::chrono::time_point<clock_> beg_;
};

template<typename _Tp> Matrix<_Tp> fill_matrix(const size_t m, const size_t n) {
	size_t i, j;
	std::vector< std::vector <_Tp> > V;

	//Type of random number distribution
	std::uniform_real_distribution<_Tp> dist(-100, 100);  //(min, max)
	//Mersenne Twister: Good quality random number generator
	std::mt19937 rng;
	//Initialize with non-deterministic seeds
	rng.seed(std::random_device{}());

	for (i = 0; i < m; ++i) {
		std::vector <_Tp> x;
		for (j = 0; j < n; ++j) {
			_Tp v;
			v = dist(rng);
			x.push_back(v);
		}
		V.push_back(x);
	}
	Matrix<_Tp> A(m, n, V);

	return A;
}
/*---------------------------------------------------------------------*/

/*---------------------------- Prototypes -----------------------------*/
template<typename _Tp> double add_matrix();
template<typename _Tp> double multiply_matrix();
template<typename _Tp> double transpose_matrix();
template<typename _Tp> double adjoint_matrix();
template<typename _Tp, typename _Dt> double inverse_matrix();
template<typename _Tp> double determinant_matrix();
/*---------------------------------------------------------------------*/


/*---------------------------- Main Routine ---------------------------*/
int main() {
	std::vector< std::string > menu = {
		"Matrix Addition",
		"Matrix Multiplication",
		"Matrix Transpose",
		"Matrix Adjoint",
		"Matrix Inverse",
		"Determinant"
	};

	std::vector< std::function<double()> > menuAction =
		std::vector<std::function<double()> >(0);
	menuAction.push_back(add_matrix<double>);
	menuAction.push_back(multiply_matrix<double>);
	menuAction.push_back(transpose_matrix<double>);
	menuAction.push_back(adjoint_matrix<double>);
	menuAction.push_back(inverse_matrix<double, float>);
	menuAction.push_back(determinant_matrix<float>);

	size_t menuCount = menu.size();

	std::cout << "\nMatrix Operation Benchmark";
	for (size_t i = 1; i <= menuCount; ++i) {
		double t;
		std::cout << std::endl << std::left << std::setw(0)
			<< i << ". ";
		std::cout << std::left << std::setfill(' ') << std::setw(25)
			<< menu.at(i - 1) << " : ";
		if (menuAction.at(i - 1) != NULL) {
			try {
				t = menuAction.at(i - 1)();
				std::cout << std::left << std::setw(0)
					<< std::scientific << std::setprecision(2)
					<< std::noshowpos << std::nouppercase << t << "ms";
			}
			catch (MatrixException &e) {
				std::cout << "Failed";
				std::cout << "\nError : " << e.what() << std::endl;
			}
		}
	}
}
/*---------------------------------------------------------------------*/


/*------------------------------ Wrappers -----------------------------*/
template<typename _Tp> double add_matrix() {
	Matrix<_Tp> A, B, C;
	double t;

	/* Fill values for Matrix A */
	A = fill_matrix<_Tp>(BEN_ORDER, BEN_ORDER);

	/* Fill values for Matrix B */
	B = fill_matrix<_Tp>(BEN_ORDER, BEN_ORDER);

	/* Perform Addition */
	{
		Timer tmr;
		C = A + B;
		tmr.reset();
		t = tmr.elapsed();
	}

	/* Return Benchmark time */
	return t;
}

template<typename _Tp> double multiply_matrix() {
	Matrix<_Tp> A, B, C;
	double t;

	/* Fill values for Matrix A */
	A = fill_matrix<_Tp>(BEN_ORDER, BEN_ORDER);

	/* Fill values for Matrix B */
	B = fill_matrix<_Tp>(BEN_ORDER, BEN_ORDER);

	/* Perform Multiplication */
	{
		Timer tmr;
		C = A * B;
		tmr.reset();
		t = tmr.elapsed();
	}

	/* Return Benchmark time */
	return t;
}

template<typename _Tp> double transpose_matrix() {
	Matrix<_Tp> A, C;
	double t;

	/* Fill values for Matrix A */
	A = fill_matrix<_Tp>(BEN_ORDER, BEN_ORDER);

	/* Perform Transpose Operation */
	{
		Timer tmr;
		C = transpose<_Tp>(A);
		tmr.reset();
		t = tmr.elapsed();
	}

	/* Return Benchmark time */
	return t;
}

template<typename _Tp> double adjoint_matrix() {
	Matrix<_Tp> A, C;
	double t;

	/* Fill values for Matrix A */
	A = fill_matrix<_Tp>(BEN_ORDER, BEN_ORDER);

	/* Perform Adjoint Operation */
	{
		Timer tmr;
		C = adjoint<_Tp>(A);
		tmr.reset();
		t = tmr.elapsed();
	}

	/* Return Benchmark time */
	return t;
}

template<typename _Tp, typename _Dt> double inverse_matrix() {
	Matrix<_Tp> A;
	Matrix<_Dt> C;
	double t;

	/* Fill values for Matrix A */
	A = fill_matrix<_Tp>(BEN_ORDER, BEN_ORDER);

	/* Perform Inverse Operation */
	{
		Timer tmr;
		C = inverse<_Tp, _Dt>(A);
		tmr.reset();
		t = tmr.elapsed();
	}

	/* Return Benchmark time */
	return t;
}

template<typename _Tp> double determinant_matrix() {
	Matrix<_Tp> A;
	_Tp det;
	double t;

	/* Fill values for Matrix A */
	A = fill_matrix<_Tp>(BEN_ORDER, BEN_ORDER);

	/* Find Determinant of Matrix */
	{
		Timer tmr;
		det = determinant<_Tp>(A);
		tmr.reset();
		t = tmr.elapsed();
	}

	/* Return Benchmark time */
	return t;
}
/*---------------------------------------------------------------------*/