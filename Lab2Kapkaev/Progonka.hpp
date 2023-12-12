#pragma once
#include "CSRMatrix.hpp"
#include "Table.hpp"

#define psi 0.5

using namespace std;

double aRazriv(double x, vector<double(*)(double x)> k,double h,double h05)
{
	return 1/(((psi - (x - h)) / (k[0](((x - h) + psi) / 2.0)) + (x - psi) / (k[1]((psi + x) / 2.0)))/h);
}

double dRazriv(double x, vector<double(*)(double x)> q, double h, double h05)
{
	double q1 = q[0](((x-h05)+psi)/2);
	double q2 = q[1]((psi+(x+h05)) / 2);

	double a = psi - (x - h05);
	double b = (x + h05) - psi;
	double c = 1.0 / h;

	return (q1*a+q2*b)*c;
}

double fiRazriv(double x, vector<double(*)(double x)> f, double h, double h05)
{
	double f1 = f[0](((x - h05) + psi) / 2);
	double f2 = f[1]((psi + (x + h05)) / 2);

	return (f1 * (psi - (x - h05)) + f2 * ((x + h05) - psi)) / h;
}

vector<double> progonkaConstStep(vector<double(*)(double x)> k, vector<double(*)(double x)> q, vector<double(*)(double x)> f,size_t n,double mu1,double mu2)
{
	MyTable* table = new MyTable();
	double h = 1.0 / n, h2 = h * h, h05 = h / 2.0, xi = h;


	std::vector<double> fi = std::vector<double>(n+1);
	CSRMatrix matrix(n+1);

	size_t i = 1, j = 0, iStop = n / 2;

	matrix[0][0] = 1;
	fi[0] = mu1;

	double ai, ai1, A, C, B, d, fr;
	ai = k[0](xi - h05);

	for (; i < iStop; i++)
	{
		xi = i * h; //переставил сюда, щас должно норм
		ai1 = k[0](xi + h05);

		A = ai / h2;
		C = (ai + ai1) / h2 + q[0](xi);// С считается без -
		B = ai1 / h2;

		matrix[i][i - 1] = A;    //A
		matrix[i][i] = -C;      //-C   // С записывается без - //я поменял, чтоб в матрицу записывало с -
		matrix[i][i + 1] = B;  //B

		fi[i] = -f[0](xi);  //fi записывается с -, в формулах надо будет не забыть разминусовать

		ai = ai1;
		   //? начинаем с x1, i не успевает поменятся и в итоге второй раз считаем с x1, отстаем на 1
	}

	xi = i * h;

	if (psi>(xi-h) && psi<xi)
	{
		ai1 = aRazriv(xi, k, h, h05);
	}
	else if (psi>=xi)
	{
		ai1 = k[0](xi - h05); //там же -0.5, исправил
	}
	else
	{
		ai1 = k[1](xi - h05); //там же -0.5, исправил
	}

	if (psi > (xi - h05) && psi < (xi+h05))
	{
		d = dRazriv(xi, q, h, h05);
		fr = fiRazriv(xi, f, h, h05);
	}
	else if (psi >= (xi+h05))
	{
		d = q[0](xi);
		fr = f[0](xi);
	}
	else
	{
		d = q[1](xi);
		fr = f[1](xi);
	}

	A = ai / h2;
	C = (ai + ai1) / h2 + d;
	B = ai1 / h2;

	matrix[i][i - 1] = A;  //A
	matrix[i][i] = -C;    //-C // С записывается без - //я поменял, чтоб в матрицу записывало с -
	matrix[i][i + 1] = B;//B
	fi[i] = -f[1](xi);  //fi записывается с -, в формулах надо будет не забыть разминусовать

	i++;
	xi = i * h;
	ai = ai1;

	//После
	while (i<n)
	{
		ai1 = k[1](xi + h05);

		matrix[i][i - 1] = ai / h2;                   //A
		matrix[i][i] = -((ai + ai1) / h2 + q[1](xi));//-C  // С записывается без - //я поменял, чтоб в матрицу записывало с -
		matrix[i][i + 1] = ai1 / h2;                //B

		fi[i] = -f[1](xi); //fi записывается с -, в формулах надо будет не забыть разминусовать

		ai = ai1;

		i++;
		xi = i * h;
	}

	fi[i] = mu2;

	matrix[i][i] = 1;

	//for (int m = 0; m <= n; m++)
	//{
	//	for (int k = 0; k <= n; k++)
	//		std::cout << matrix[m][k] <<" ";
	//	std::cout<< std::endl;
	//}
	
	//Прогонка

	vector<double> alfa(n), beta(n),y(n+1);
	alfa[0] = 0.0;

	beta[0] = mu1;  // правое значение константа поменял на mu1

	for (i = 1; i < n; i++)
	{
		double m = matrix[i][i + 1];
		double k = -matrix[i][i] - matrix[i][i - 1] * alfa[i - 1]; //Разминусовал ранее минусованное С

		alfa[i] = m/k;

		m = -fi[i] + matrix[i][i - 1] * beta[i - 1];         //у нас в матрице fi с минусами значит при записи сюда разминусовываем
		k = -matrix[i][i] - matrix[i][i - 1] * alfa[i - 1]; //Разминусовал ранее минусованное С
		beta[i] = m/k;
	}

	y[n] = mu2;

	for (i = n-1; i >0; i--)
	{
		y[i] = alfa[i] * y[i+1] + beta[i];
	}

	y[0] = mu1;

	std::cout << "result: "<< std::endl;
	for (int m = 0; m <= n; m++)
	{
		std::cout << y[m] << std::endl; //поменял fi на y чисто посмотреть
	}
	std::cout << std::endl;

	return y;
}
