#include <iostream>
#include <SparseCore>
#include <SparseLU>
#include <IterativeLinearSolvers>

void solverFunction()
{
	Eigen::SparseMatrix<double> A(2, 2);
	A.insert(0, 0) = 1;
	A.insert(0, 1) = 2;
	A.insert(1, 0) = 3;
	A.insert(1, 1) = 4;

	Eigen::Vector2d b;
	Eigen::Vector2d x;
	b[0] = 1;
	b[1] = 2;

	Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
	solver.compute(A);
	if (solver.info() != Eigen::ComputationInfo::Success)
	{
		std::cerr << "failed to compute" << std::endl;
		return;
	}
	x = solver.solve(b);
	if (solver.info() != Eigen::ComputationInfo::Success)
	{
		std::cerr << "failed to solve" << std::endl;
		return;
	}
	std::cout << x << std::endl;

	A.block(0,0,1,1) = 100;

	std::cout << A.block(0, 0, 1, 1) << std::endl;
}

int main()
{
	solverFunction();
}
