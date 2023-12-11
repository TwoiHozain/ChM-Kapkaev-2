#pragma once
#include "Progonka.hpp"

double k0(double x) { return 1.0; }
double k1(double x) { return 1.0; }

double q0(double x) { return 2.0; }
double q1(double x) { return 1.0; }

double f0(double x) { return 0.0; }
double f1(double x) { return 1.0; }

#define e exp(1)
#define e1 exp(-1)

void testTaskConstStep()
{
	size_t n = 3000;
	int i = 1;

	vector<double> u;
	double  h = 1.0 / n, x = h,mu1=3.0,mu2=4.0;
	double C2 = (mu2 - 1 - mu1 * e) / (e1 - e),C1=mu1-C2;
	int iStop = n / 2;

	u.push_back(mu1);
	for (; i <= iStop; i++)
	{
		u.push_back(C1*exp(1.414213562373*x)+ C2*exp(-1.414213562373 * x));
		x = i * h;
	}

	for (; i < n; i++)
	{
		u.push_back(C1 * exp(x) + C2 * exp(-x)+1);
		x = i * h;
	}
	u.push_back(mu2);

	vector<double> v = progonkaConstStep({&k0,&k1}, { &q0,&q1}, { &f0,&f1 }, n, mu1, mu2);

	//vector<double> v = {
	//	3.0,
	//	2.9130467825976922113,
	//	2.85581853236474863,
	//	2.8277312875640984017,
	//	2.8284984436569593821,
	//	2.8581278287667281115,
	//	2.9169217835577899849,
	//	3.0054802463443395114,
	//	3.1119377926488304117,
	//	3.229170531772958202,
	//	3.3577765899367439423,
	//	3.4984121204981661314,
	//	3.6517946516743748825,
	//	3.8187067473999426894,
	//	4.0};

	for (i = 0; i <= n; i++)
		cout << u[i] - v[i] <<std::endl;
}