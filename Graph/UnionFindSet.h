#pragma once

class UnionFindSet
{
public:
	UnionFindSet(size_t n)
	{
		v.resize(n + 1, -1);
	}

	size_t FindRoot(size_t x)
	{
		size_t ret = x;

		while (v[ret] >= 0)
			ret = v[ret];

		return ret;
	}

	void Union(size_t x1, size_t x2)
	{
		size_t root1 = FindRoot(x1);
		size_t root2 = FindRoot(x2);
		if (root1 == root2)
			return;

		v[root1] += v[root2];
		v[root2] = root1;
	}

	bool IsInSet(int x1, int x2)
	{
		return FindRoot(x1) == FindRoot(x2);
	}

	size_t SetSize()
	{
		size_t count = 0;
		for (size_t i = 0; i < v.size(); ++i)
		{
			if (v[i] < 0)
				count++;
		}

		return count - 1;
	}
protected:
	vector<int> v;
};

void TestUnionFindSet()
{
	const int n = 5;
	const int m = 4;

	UnionFindSet ufs(n);
	int r[m][2] = { { 1, 2 },{ 2, 3 },{ 4, 5 },{ 2, 4 } };
	for (size_t i = 0; i < m; ++i)
	{
		ufs.Union(r[i][0], r[i][1]);
	}

	cout << "朋友圈的个数为:" << ufs.SetSize() << endl;
}