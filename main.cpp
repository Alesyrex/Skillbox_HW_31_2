#include <iostream>
#include <vector>
#include <map>

class IGraph {
public:
    IGraph() {}
    IGraph(const IGraph& _oth){}
    virtual void AddEdge(int from, int to) = 0;
    virtual int VerticesCount() const = 0;
    virtual std::vector<int> Vertices() const = 0;
    virtual void GetNextVertices(int vertex, std::vector<int> &vertices) const = 0;
    virtual void GetPrevVertices(int vertex, std::vector<int> &vertices) const = 0;
    virtual ~IGraph() {}
    virtual void print () const = 0;
};

class ListGraph : public IGraph
{
    std::map<int,std::vector<int>> _nextVert;
    std::map<int,std::vector<int>> _prevVert;
public:
    ListGraph() {}
    ListGraph(const IGraph& _oth)
    {
        for (int i=0;i < _oth.VerticesCount();++i)
        {
            std::vector<int> tmp;
            _oth.GetNextVertices(i, tmp);
            _nextVert.insert(std::pair<int, std::vector<int>>(_oth.Vertices()[i], tmp));
        }
        for (int i=0;i < _oth.VerticesCount();++i)
        {
            std::vector<int> tmp;
            _oth.GetPrevVertices(i, tmp);
            _prevVert.insert(std::pair<int, std::vector<int>>(_oth.Vertices()[i], tmp));
        }
    }

    ListGraph& operator=(const IGraph& _oth)
    {
        if(this == &_oth)
            return *this;

        for (int i=0;i < _oth.VerticesCount();++i)
        {
            std::vector<int> tmp;
            _oth.GetNextVertices(i, tmp);
            _nextVert.insert(std::pair<int, std::vector<int>>(_oth.Vertices()[i], tmp));
        }
        for (int i=0;i < _oth.VerticesCount();++i)
        {
            std::vector<int> tmp;
            _oth.GetPrevVertices(i, tmp);
            _prevVert.insert(std::pair<int, std::vector<int>>(_oth.Vertices()[i], tmp));
        }
        return *this;
    }

    virtual void AddEdge(int from, int to) override
    {
        std::vector<int> vertices_tmp;
        vertices_tmp.push_back(to);
        auto it_next = _nextVert.find(from);
        if (it_next == _nextVert.end())
            _nextVert.insert(std::pair<int, std::vector<int>>(from, vertices_tmp));
        else
            it_next->second.push_back(to);

        vertices_tmp.clear();
        vertices_tmp.push_back(from);
        auto it_prev = _prevVert.find(to);
        if (it_prev == _prevVert.end())
            _prevVert.insert(std::pair<int, std::vector<int>>(to, vertices_tmp));
        else
            it_prev->second.push_back(from);
    }
    virtual int VerticesCount() const override
    {
        return Vertices().size();
    }
    virtual void GetNextVertices(int vertex, std::vector<int> &vertices) const override
    {
        auto it = _nextVert.find(vertex);
        for (int i=0;i < it->second.size();++i)
        {
            vertices.push_back(it->second[i]);
        }
    }
    virtual void GetPrevVertices(int vertex, std::vector<int> &vertices) const override
    {
        auto it = _prevVert.find(vertex);
        for (int i=0;i < it->second.size();++i)
        {
            vertices.push_back(it->second[i]);
        }
    }

    virtual std::vector<int> Vertices() const override
    {
        std::vector<int> tmp;
        for (auto it = _nextVert.begin();it != _nextVert.end();++it)
        {
            tmp.push_back(it->first);
        }
        return tmp;
    }

    virtual void print () const override
    {
        for (auto it = _nextVert.begin();it != _nextVert.end();++it)
        {
            std::cout << it->first << ": ";
            for (int i=0;i < it->second.size();++i)
            {
                std::cout << it->second[i];
                if (i+1 != it->second.size())
                    std::cout << "->";
            }
            std::cout << std::endl;
        }
    }
};

class MatrixGraph : public IGraph
{
    std::vector<int> indexToVertex;
    std::map<int,int> vertexToIndex;
    std::vector<std::vector<int>> matrix;
public:
    MatrixGraph() {}
    MatrixGraph(const IGraph& _oth)
    {
        for (int i=0;i < _oth.VerticesCount();++i)
        {
            std::vector<int> tmp;
            _oth.GetNextVertices(_oth.Vertices()[i], tmp);
            for (int j=0;j < tmp.size();++j)
            {
                AddEdge(_oth.Vertices()[i],tmp[j]);
            }
        }
    }

    MatrixGraph& operator= (const IGraph& _oth)
    {
        if(this == &_oth)
            return *this;

        for (int i=0;i < _oth.VerticesCount();++i)
        {
            std::vector<int> tmp;
            _oth.GetNextVertices(_oth.Vertices()[i], tmp);
            for (int j=0;j < tmp.size();++j)
            {
                AddEdge(_oth.Vertices()[i],tmp[j]);
            }

        }
        return *this;
    }

    int getIndex(int vertex)
    {
        auto it = vertexToIndex.find(vertex);
        if (it != vertexToIndex.end())
            return it->second;

        const int res = indexToVertex.size();

        indexToVertex.push_back(vertex);
        vertexToIndex[vertex] = res;

        for(auto &pow: matrix)
            pow.push_back(0);

        matrix.emplace_back(res+1,0);
        return res;
    }

    virtual void AddEdge(int from, int to) override {
        auto from_ind = getIndex(from);
        auto to_ind = getIndex(to);

        ++matrix[to_ind][from_ind];
    }

    virtual int VerticesCount() const override
    {
        return indexToVertex.size();
    }

    virtual std::vector<int> Vertices() const override
    {
        return indexToVertex;
    }

    virtual void GetNextVertices(int vertex, std::vector<int> &vertices) const override
    {
        for (int i=0;i < matrix.size();++i)
        {
            if (matrix[i][vertex])
                for (int j=0;j < matrix[i][vertex];++j)
                    vertices.push_back(indexToVertex[i]);
        }
    }
    virtual void GetPrevVertices(int vertex, std::vector<int> &vertices) const override
    {
        for (int i=0;i < matrix.size();++i)
        {
            if (matrix[vertex][i])
            {
                for (int j=0;j < matrix[vertex][i];++j)
                    vertices.push_back(indexToVertex[i]);
            }
        }
    }
    virtual void print () const override
    {
        for (int i=0;i < matrix.size();++i)
        {
            for (int j=0;j < matrix.size();++j)
            {
                std::cout << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    MatrixGraph g1;

    g1.AddEdge(1, 2);
    g1.AddEdge(1, 4);
    g1.AddEdge(1, 2);
    g1.AddEdge(20, 20);
    g1.AddEdge(20, 20);
    g1.AddEdge(-1, 1);

    ListGraph g2;
    g2 = g1;
    g2 = g2;
    MatrixGraph g3(g2);

    return 0;
}
