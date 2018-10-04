#pragma once
#include <vector>

class QuickSet
{
	int lx;
	std::vector<int> x{};
	std::vector<int> nx{};

public:
	explicit QuickSet(int maximumSize);
	void Clear();
	int Size() const;
	bool Contains(int id) const;
	void Insert(int id);
	void Remove(int id);
	std::vector<int> Items();
	std::vector<int> Inverse() const;
};

