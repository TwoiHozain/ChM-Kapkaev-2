#pragma once
#include "Progonka.hpp"
#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

double k0(double x) { return 1.0; }
double k1(double x) { return 1.0; }

double q0(double x) { return 2.0; }
double q1(double x) { return 1.0; }

double f0(double x) { return 0.0; }
double f1(double x) { return 1.0; }

vector<double>* getU(double mu1,double mu2,size_t n, Container<double>& C)
{
	vector<double>* u = new vector<double>;
	double h = 1.0 / n,x=h;
	int i = 1, iStop = n/2;

	(*u).push_back(mu1);
	for (; i <= iStop; i++)
	{
		x = i * h;
		(*u).push_back(C[0] * exp(1.414213562373095 * x) + C[1] * exp(-1.414213562373095 * x));
	}

	for (; i < n; i++)
	{
		x = i * h;
		(*u).push_back(C[2] * exp(x) + C[3] * exp(-x) + 1);
	}

	(*u).push_back(mu2);

	return u;
}

void testTaskConstStep(size_t n,double mu1,double mu2)
{
	int i = 1;

	double  h = 1.0 / n, x = h;
	int iStop = n / 2;
	
	Container<double> C = getC(mu1, mu2);

	vector<double>* u = getU(mu1, mu2, n, C);
	vector<double>* v = progonkaConstStep({&k0,&k1}, { &q0,&q1}, { &f0,&f1 }, n, mu1, mu2);
	vector<double> xi;

	double maxuminsuv = (*u)[0] - (*v)[0];

	for (i = 1; i <= n; i++)
	{

		if (abs((*u)[i] - (*v)[i]) > maxuminsuv)
		{
			maxuminsuv = abs((*u)[i] - (*v)[i]);
		}
	}

	std::ofstream out("defaulOut.txt");

	size_t width = 13;
	out << std::setw(width) <<"Max |u(xi) - v(xi)| " << maxuminsuv << endl;

	out << std::setw(width) << "N" << "|";
	out << std::setw(width) << "Xi" << "|";
	out << std::setw(width) << "U(xi)" << "|";
	out << std::setw(width) << "V(xi)" << "|";
	out << std::setw(width) << "U(xi)-V(xi)" << "|";

	x = i * h;

	if (out.is_open())
	{
		for (int i = 0; i <= n; i++)
		{
			out << std::right << std::setw(width) << i << "|";
			out << std::right << std::setw(width) << i*h << "|";
			xi.push_back(i * h);
			out << std::right << std::setw(width) << (*u)[i] << "|";
			out << std::right << std::setw(width) << (*v)[i] << "|";
			out << std::right << std::setw(width) << (*u)[i] - (*v)[i] << "|";
			out << std::endl;
		}
	}
	out.close();

	plt::suptitle("Test task");
	plt::xlabel("X");
	plt::ylabel("V / U");
	plt::plot(xi, (*v), { {"color", "red"}, { "linestyle", "--" } });
	plt::plot(xi, (*u), { {"color", "blue"}, { "linestyle", ":" } });
	plt::show();
}