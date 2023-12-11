#pragma once

#include <ios>
#include <iomanip>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <vector>
#include <string>

class MyTable
{
	std::vector<std::string> columnNames;
	std::vector<std::vector<double>> columns;
public:
	MyTable() {}

	MyTable(const std::vector<std::string>& rowNames)
	{
		this->columnNames = rowNames;
		columns = std::vector<std::vector<double>>(rowNames.size());
	}

	void addRow(const std::vector<double>& row)
	{
		for (int i = 0; i < columns.size(); i++)
			columns[i].push_back(row[i]);
	}

	void addColumn(std::string columnName, std::vector<double> values)
	{
		columnNames.push_back(columnName);
		columns.push_back(values);
	}

	void printTable()
	{
		size_t width = 13;

		for (int i = 0; i < columnNames.size(); i++)
			std::cout << std::setw(width) << columnNames[i] << "|";

		std::cout << std::endl;

		for (int i = 0; i < columns[0].size(); i++)
		{
			for (int j = 0; j < columns.size(); j++)
				std::cout << std::right << std::setw(width) << columns[j][i] << "|";
			std::cout << std::endl;
		}
	}

	void saveToTXT(std::string outPath = "defaulOut.txt")
	{
		std::ofstream out(outPath);

		size_t width = 13;

		if (out.is_open())
		{
			for (int i = 0; i < columnNames.size(); i++)
				out << std::setw(width) << columnNames[i] << "|";

			out << std::endl;

			for (int i = 0; i < columns[0].size(); i++)
			{
				for (int j = 0; j < columns.size(); j++)
					out << std::right << std::setw(width) << columns[j][i] << "|";
				out << std::endl;
			}
		}
		out.close();
	}

	std::vector<double> getColumn(std::string columnName)
	{
		size_t columnInd;

		for (size_t i = 0; i < columnNames.size(); i++)
			if (columnNames[i] == columnName)
				columnInd = i;

		return columns[columnInd];
	}

	~MyTable();
};
