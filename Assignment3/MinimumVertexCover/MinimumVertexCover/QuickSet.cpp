#include "stdafx.h"
#include "QuickSet.h"
#include <iostream>

QuickSet::QuickSet(int maximumSize)
{
	lx = 0;
	x = std::vector<int>(maximumSize, 0);
	nx = std::vector<int>(maximumSize, -1);
}

void QuickSet::Clear()
{
	// Take advantage of vector operations if lx larger than threshold
	// std::fill can fill 16-64 bits per vector op
	if (lx > 128)
	{
		std::fill(nx.begin(), nx.end(), -1);
		lx = 0;
	} 
	else
	{
		for (auto i = 0; i < lx; i++)
		{
			nx.at(x.at(i)) = -1;
		}
		lx = 0;
	}
}

int QuickSet::Size() const
{
	return lx;
}

bool QuickSet::Contains(int id) const
{
	return nx.at(id) != -1;
}

void QuickSet::Insert(int id)
{
	if (nx.at(id) != -1) return;
	nx.at(id) = lx;
	x.at(lx) = id;
	lx++;
}

void QuickSet::Remove(int id)
{
	if (id < 0 || id > x.size()) std::cout << "Broken ID: " << id;
	if (nx.at(id) == -1) return;
	const auto position = nx.at(id);
	x.at(position) = x.at(lx - 1);
	nx.at(id) = -1;
	lx--;
}

std::vector<int> QuickSet::Items()
{
	const auto first = x.begin();
	const auto last = x.begin() + lx;
	return std::vector<int>(first, last);
}

std::vector<int> QuickSet::Inverse() const
{
	auto inverse = std::vector<int>();
	inverse.reserve(x.size() - lx);
	for (auto i = 0; i < nx.size(); i++)
	{
		if (nx.at(i) == -1) inverse.emplace_back(i);
	}
	return inverse;
}





