#pragma once

template<typename V, typename W>
class GraphMatrix
{
public:
	GraphMatrix(V* vertexs, size_t n, const W& invalid = W(), bool IsDirected = false)
		:_vertexs(vertexs, vertexs + n)
		, _isDirected(IsDirected)
	{
		_matrix = new W*[n];
		for (size_t i = 0; i < n; ++i)
		{
			_matrix[i] = new W[n];
			for (size_t j = 0; j < n; ++j)
			{
				_matrix[i][j] = invalid;
			}
		}
	}

	~GraphMatrix()
	{}

	int GetIndex(const V& v)
	{
		for (size_t i = 0; i < _vertexs.size(); ++i)
		{
			if (_vertexs[i] == v)
				return i;
		}
		assert(false);
		return -1;
	}

	void AddEdge(const V& v1, const V& v2, const W& w)
	{
		size_t src = GetIndex(v1);
		size_t des = GetIndex(v2);
		_matrix[src][des] = w;

		if (_isDirected == false)
			_matrix[des][src] = w;
	}
protected:
	vector<V> _vertexs;
	W** _matrix;
	bool _isDirected;
};

void TestGraphMatrix()
{
	string city[] = { "北京", "上海", "广州", "杭州", "西安" };
	GraphMatrix<string, double> gpm(city, sizeof(city) / sizeof(city[0]));
	gpm.AddEdge("北京", "上海", 300.3);
	gpm.AddEdge("北京", "广州", 850.5);
	gpm.AddEdge("北京", "杭州", 299);
	gpm.AddEdge("北京", "西安", 475);
}

//邻接表
template<typename W>
struct Edge
{
	W _weight;
	Edge<W> *_next;
	size_t _src;
	size_t _des;

	Edge(size_t src, size_t des, const W& weight)
		:_weight(weight)
		, _next(NULL)
		, _src(src)
		, _des(des)
	{}
};

template<typename V, typename W>
class GraphLink
{
	typedef Edge<W> Node;
public:
	GraphLink()
	{}

	GraphLink(V* vertexs, size_t n, bool IsDirected = false)
	{
		_vertexs.resize(n);
		for (size_t i = 0; i < n; ++i)
		{
			_vertexs[i] = vertexs[i];
		}
		_linkTables.resize(n, NULL);
	}

	~GraphLink()
	{}

	int GetIndex(const V& v)
	{
		for (size_t i = 0; i < _vertexs.size(); ++i)
		{
			if (v == _vertexs[i])
				return i;
		}
		assert(false);
		return -1;
	}

	V& GetV(const size_t index)
	{
		assert(index < _vertexs.size());
		return _vertexs[index];
	}
	void _AddEdge(const size_t& src, const size_t& des, const W&w)
	{
		Node* newNode = new Node(src, des, w);
		newNode->_next = _linkTables[src];
		_linkTables[src] = newNode;
	}
	void AddEdge(const V& v1, const V& v2, const W& w)
	{
		size_t src = GetIndex(v1);
		size_t des = GetIndex(v2);
		_AddEdge(src, des, w);

		if (_isDirected == false)
			_AddEdge(des, src, w);
	}

	void BFS(const V& src)
	{
		queue<size_t> q;
		q.push(GetIndex(src));
		set<size_t> s;
		s.insert(GetIndex(src));
		while (!q.empty())
		{
			size_t tmp = q.front();
			cout << GetV(tmp) << " ";
			q.pop();
			Node* cur = _linkTables[tmp];
			while (cur)
			{
				pair<set<size_t>::iterator, bool> ret = s.insert(cur->_des);
				if (ret.second == true)
					q.push(*ret.first);

				cur = cur->_next;
			}
		}
		cout << endl;
	}

	void BFS_ByVector(const V&src)
	{
		queue<size_t> q;
		size_t isrc = GetIndex(src);
		q.push(isrc);
		vector<bool> v;
		v.resize(_vertexs.size(), false);
		v[isrc] = true;

		while (!q.empty())
		{
			size_t tmp = q.front();
			cout << GetV(tmp) << " ";
			q.pop();
			Node* cur = _linkTables[tmp];
			while (cur)
			{
				size_t dst = cur->_des;
				if (v[dst] == false)
				{
					v[dst] = true;
					q.push(dst);
				}
				cur = cur->_next;
			}
		}
		cout << endl;
	}

	void DFS(const V& src)
	{
		size_t isrc = GetIndex(src);
		set<size_t> s;
		s.insert(isrc);
		_DFS(isrc, s);
		cout << endl;
	}

	void DFS_ByVector(const V& src)
	{
		size_t isrc = GetIndex(src);
		vector<bool> v;
		v.resize(_vertexs.size(), false);
		v[isrc] = true;
		_DFS_ByVector(isrc, v);
		cout << endl;
	}

	bool Kruskal(GraphLink<V, W>& mintree)
	{
		mintree._vertexs = _vertexs;
		mintree._linkTables.resize(_vertexs.size(), NULL);
		mintree._isDirected = _isDirected;

		struct Compare
		{
			bool operator()(Node* l, Node* r)
			{
				return l->_weight < r->_weight;
			}
		};

		priority_queue<Node*, vector<Node*>, Compare> pq;
		for (size_t i = 0; i < _vertexs.size(); ++i)
		{
			Node* cur = _linkTables[i];
			while (cur)
			{
				if (cur->_src < cur->_des)
					pq.push(cur);

				cur = cur->_next;
			}
		}

		UnionFindSet ufs(_vertexs.size());

		while (!pq.empty())
		{
			Node* top = pq.top();
			pq.pop();
			size_t isrc = top->_src;
			size_t ides = top->_des;
			if (ufs.IsInSet(isrc, ides) == false)
			{
				mintree._AddEdge(isrc, ides, top->_weight);
				mintree._AddEdge(ides, isrc, top->_weight);
				ufs.Union(isrc, ides);
				if (ufs.SetSize() == 1)
					return true;
			}

		}
		return false;
	}
	size_t countLink()
	{
		size_t count = 0;
		for (size_t i = 0; i < _vertexs.size(); ++i)
		{
			Node* cur = _linkTables[i];
			while (cur)
			{
				if (cur->_src < cur->_des)
				{
					cout << cur->_src << ":" << cur->_des << endl;
					count++;
				}
				cur = cur->_next;
			}
		}
		return count;
	}
protected:
	void _DFS_ByVector(const size_t src, vector<bool>&v)
	{
		Node* cur = _linkTables[src];
		cout << GetV(src) << " ";
		while (cur)
		{
			size_t dst = cur->_des;
			if (v[dst] == false)
			{
				v[dst] = true;
				_DFS_ByVector(dst, v);
			}
			cur = cur->_next;
		}
	}

	void _DFS(const size_t src, set<size_t> &s)
	{
		Node* cur = _linkTables[src];
		cout << GetV(src) << " ";
		while (cur)
		{
			pair<set<size_t>::iterator, bool> ret = s.insert(cur->_des);
			if (ret.second == true)
				_DFS(cur->_des, s);

			cur = cur->_next;
		}
	}
protected:
	vector<V> _vertexs;
	vector<Node*> _linkTables;
	bool _isDirected;
};

void TestGraphLink()
{
	string city[] = { "北京", "上海", "广州", "杭州", "西安" };
	GraphLink<string, double> gpm(city, sizeof(city) / sizeof(city[0]));

	gpm.AddEdge("北京", "杭州", 299);
	gpm.AddEdge("北京", "西安", 475);
	gpm.AddEdge("上海", "杭州", 850.5);
	gpm.AddEdge("上海", "西安", 475);
	gpm.AddEdge("西安", "广州", 300.3);
	gpm.AddEdge("上海", "广州", 300.3);

	gpm.BFS("北京");
	gpm.BFS_ByVector("北京");
	gpm.DFS("北京");
	gpm.DFS_ByVector("北京");

	GraphLink<string, double> mintree;
	cout << "mintree:" << gpm.Kruskal(mintree) << endl;
	cout << mintree.countLink() << endl;
}