﻿// Lab2Kapkaev.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "TestTask.hpp"
#include "MainTask.hpp"

int main()
{
	size_t n;
	double mu1, mu2,eps;
	char choise;
	
	//cout << "Choose task:\n0 - test task;\n1 - main task.\n";
	//cin >> choise;


	mainTaskVariableStep(100, 1.0, 1.0, 0.001);
	//switch (choise)
	//{
	//case('0'):
	//	cout << "Enter mu1\n";
	//	cin >> mu1;
	//	cout << "Enter mu2\n";
	//	cin >> mu2;
	//	cout << "Enter n\n";
	//	cin >> n;

	//	testTaskConstStep(n,mu1,mu2);
	//	break;
	//case('1'):
	//	cout << "Enter mu1\n";
	//	cin >> mu1;
	//	cout << "Enter mu2\n";
	//	cin >> mu2;
	//	cout << "Enter n\n";
	//	cin >> n;
	//	cout << "Enter eps\n";
	//	cin >> eps;

	//	mainTaskVariableStep(n, mu1, mu2,eps);
	//	break;
	//default:
	//	cout << "Uncnown choise";
	//	break;
	//}

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
