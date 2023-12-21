#pragma once
#include "Progonka.hpp"
#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

double k01(double x) { return exp(-x) * exp(1.0 / 2.0); }
double k11(double x) { return 1.0; }

double q01(double x) { return 2.0; }
double q11(double x) { return sin(3.14159265359 * x); }

double f01(double x) { return cos(3.14159265359 * x); }
double f11(double x) { return exp(x) / exp(1.0 / 2.0); }

void mainTaskVariableStep(size_t nStart,double mu1,double mu2,double eps)
{
	size_t n = nStart;
	Container<double> C = getC(mu1, mu2);
	double maxRaznost = -1.0;
	double h = 1.0 / n;

	vector<double>* v = progonkaConstStep({ &k01,&k11 }, { &q01,&q11 }, { &f01,&f11 }, n, mu1, mu2);
	vector<double>* v2 = progonkaConstStep({ &k01,&k11 }, { &q01,&q11 }, { &f01,&f11 }, n*2, mu1, mu2);
	vector<double> v12;

	for (int i = 0; i < n; i++)
	{
		if (abs((*v)[i] - (*v2)[i * 2]) > maxRaznost)
		{
			maxRaznost = abs((*v)[i] - (*v2)[i * 2]);
		}
	}

	while (maxRaznost > eps)
	{
		maxRaznost = -1.0;
		n *= 2;
		v = progonkaConstStep({ &k01,&k11 }, { &q01,&q11 }, { &f01,&f11 }, n, mu1, mu2);
		v2 = progonkaConstStep({ &k01,&k11 }, { &q01,&q11 }, { &f01,&f11 }, n * 2, mu1, mu2);

		for (int i = 0; i < n; i++)
		{
			if (abs((*v)[i] - (*v2)[i * 2]) > maxRaznost)
			{
				maxRaznost = abs((*v)[i] - (*v2)[i * 2]);
			}
		}
	}

	vector<double> xi;
	vector<double> x2i;

	for (int i = 0; i <= n; i++)
	{
		xi.push_back(i*h);
	}

	double h2 = 1.0 / (n * 2);
	for (int i = 0; i <= n*2; i++)
	{
		x2i.push_back(i * h2);
	}

	std::ofstream out("defaulOut.txt");

	size_t width = 13;
	out << std::setw(width) << "Max |u(xi) - v(xi)| " << maxRaznost << endl;

	out << std::setw(width) << "N" << "|";
	out << std::setw(width) << "Xi" << "|";
	out << std::setw(width) << "v(xi)" << "|";
	out << std::setw(width) << "V2(xi)" << "|";
	out << std::setw(width) << "v(xi)-V2(xi)" << "|";

	if (out.is_open())
	{
		for (int i = 0; i <= n; i++)
		{
			out << std::right << std::setw(width) << i << "|";
			out << std::right << std::setw(width) << xi[i] << "|";
			out << std::right << std::setw(width) << (*v)[i] << "|";
			out << std::right << std::setw(width) << (*v2)[i*2] << "|";
			out << std::right << std::setw(width) << (*v)[i] - (*v2)[i*2] << "|";
			out << std::endl;
		}
	}
	out.close();

	plt::suptitle("Test task");
	plt::xlabel("X");
	plt::ylabel("V / V2");
	plt::plot(xi, (*v), { {"color", "red"}, { "linestyle", "--" } });
	plt::plot(x2i, (*v2), { {"color", "blue"}, { "linestyle", ":" } });
	plt::show();
}
