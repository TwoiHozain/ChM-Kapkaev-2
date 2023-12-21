#pragma once
#include "CSRMatrix.hpp"
#include "Table.hpp"
#include "Gauss.hpp"

#define psi 0.5
#define myE exp(1)
#define myE1 exp(-1)

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

Container<double> getC(double mu1, double mu2)
{
	Container<double> B(4);
	B[0] = mu1;
	B[1] = 1.0;
	B[2] = mu2 - 1.0;
	B[3] = 0.0;

	Matrix<double> A(4);

	A[0][0] = 1.0; A[0][1] = 1.0; A[0][2] = 0.0; A[0][3] = 0.0;
	A[1][0] = 2.0281149816474726; A[1][1] = 0.4930686913952398; A[1][2] = -1.6487212707001282; A[1][3] = -0.6065306597126334;
	A[2][0] = 0.0; A[2][1] = 0.0; A[2][2] = myE; A[2][3] = myE1;
	A[3][0] = -2.868187713097916; A[3][1] = 0.6973044305527023; A[3][2] = 1.6487212707001282; A[3][3] = -0.6065306597126334;

	SLAE slae(A, B, 4);

	slae.solveByGauss();
	return slae.getSolution();
}

void SystemInit(CSRMatrix& matrix, vector<double> &fi,vector<double(*)(double x)> k, vector<double(*)(double x)> q, 
	vector<double(*)(double x)> f, size_t n, double mu1, double mu2)
{
	size_t i = 1, j = 0, iStop = n / 2;
	double h = 1.0 / n, h2 = h * h, h05 = h / 2.0, xi = h;

	matrix[0][0] = 1;
	fi[0] = mu1;

	double ai, ai1, A, C, B, d, fr;
	ai = k[0](xi - h05);

	for (; i < iStop; i++)
	{
		xi = i * h; //���������� ����, ��� ������ ����
		ai1 = k[0](xi + h05);

		A = ai / h2;
		C = (ai + ai1) / h2 + q[0](xi);// � ��������� ��� -
		B = ai1 / h2;

		matrix[i][i - 1] = A;    //A
		matrix[i][i] = C;      //-C   // � ������������ ��� - //� �������, ���� � ������� ���������� � -
		matrix[i][i + 1] = B;  //B

		fi[i] = f[0](xi);  //fi ������������ � -, � �������� ���� ����� �� ������ �������������

		ai = ai1;
		//? �������� � x1, i �� �������� ��������� � � ����� ������ ��� ������� � x1, ������� �� 1
	}

	xi = i * h;

	if (psi > (xi - h) && psi < xi)
	{
		ai1 = aRazriv(xi, k, h, h05);
	}
	else if (psi >= xi)
	{
		ai1 = k[0](xi - h05); //��� �� -0.5, ��������
	}
	else
	{
		ai1 = k[1](xi - h05); //��� �� -0.5, ��������
	}

	if (psi > (xi - h05) && psi < (xi + h05))
	{
		d = dRazriv(xi, q, h, h05);
		fr = fiRazriv(xi, f, h, h05);
	}
	else if (psi >= (xi + h05))
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
	matrix[i][i] = C;    //-C // � ������������ ��� - //� �������, ���� � ������� ���������� � -
	matrix[i][i + 1] = B;//B
	fi[i] = f[1](xi);  //fi ������������ � -, � �������� ���� ����� �� ������ �������������

	i++;
	xi = i * h;
	ai = ai1;

	//�����
	while (i < n)
	{
		ai1 = k[1](xi + h05);

		matrix[i][i - 1] = ai / h2;                   //A
		matrix[i][i] = ((ai + ai1) / h2 + q[1](xi));//-C  // � ������������ ��� - //� �������, ���� � ������� ���������� � -
		matrix[i][i + 1] = ai1 / h2;                //B

		fi[i] = f[1](xi); //fi ������������ � -, � �������� ���� ����� �� ������ �������������

		ai = ai1;

		i++;
		xi = i * h;
	}

	fi[i] = mu2;

	matrix[i][i] = 1;
}

vector<double>* progonka(CSRMatrix& matrix, vector<double>& fi, size_t n, double mu1, double mu2)
{
	vector<double> alfa(n), beta(n);
	vector<double>* y = new vector<double>(n + 1);
	size_t i = 1;

	alfa[0] = 0.0;
	beta[0] = mu1;  // ������ �������� ��������� ������� �� mu1

	for (i = 1; i < n; i++)
	{
		alfa[i] = (matrix[i][i + 1]) / (matrix[i][i] - matrix[i][i - 1] * alfa[i - 1]);
		beta[i] = (fi[i] + matrix[i][i - 1] * beta[i - 1]) / (matrix[i][i] - matrix[i][i - 1] * alfa[i - 1]);
	}

	(*y)[n] = mu2;

	for (i = n - 1; i > 0; i--)
	{
		(*y)[i] = alfa[i] * (*y)[i + 1] + beta[i];
	}

	(*y)[0] = mu1;

	return y;
}

vector<double>* progonkaConstStep(vector<double(*)(double x)> k, vector<double(*)(double x)> q, vector<double(*)(double x)> f,size_t n,double mu1,double mu2)
{
	MyTable* table = new MyTable();
	double h = 1.0 / n, h2 = h * h, h05 = h / 2.0, xi = h;

	std::vector<double> fi = std::vector<double>(n+1);
	CSRMatrix matrix(n+1);

	size_t i = 1, j = 0, iStop = n / 2;
	
	SystemInit(matrix,fi,k,q,f,n,mu1,mu2);

	return progonka(matrix, fi, n, mu1, mu2);
}

//vector<double>* progonkaVariableStep()
//{
//
//}