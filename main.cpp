#include <iostream>
#include <vector>
#include <map>

class IGraph {
public:
    IGraph() {}
    IGraph(const IGraph& _oth){}
    virtual void AddEdge(int from, int to) = 0;
    virtual int VerticesCount() const = 0;
    virtual void GetNextVertices(int vertex, std::vector<int> &vertices) const = 0;
    virtual void GetPrevVertices(int vertex, std::vector<int> &vertices) const = 0;
    virtual ~IGraph() {}
    virtual void print () const = 0;
};

class ListGraph : public IGraph
{
    std::map<int,std::vector<int>> list;
public:
    ListGraph() {}
    ListGraph(const IGraph& _oth)
    {
        for (int i=1;i <= _oth.VerticesCount();++i)
        {
            std::vector<int> tmp;
            _oth.GetNextVertices(i, tmp);
            list.insert(std::pair<int, std::vector<int>>(i, tmp));
        }
    }
    virtual void AddEdge(int from, int to) override
    {
        if (from <= 0 || to <= 0)
            std::cout << "values must be greater than zero!" << std::endl;
        else
        {
            std::vector<int> vertices_tmp;
            vertices_tmp.push_back(to);
            auto it = list.find(from);
            if (it == list.end())
                list.insert(std::pair<int, std::vector<int>>(from, vertices_tmp));
            else
                it->second.push_back(to);
        }
    }
    virtual int VerticesCount() const override
    {
        auto it = list.rbegin();
        return it->first;
    }
    virtual void GetNextVertices(int vertex, std::vector<int> &vertices) const override
    {
        auto it = list.find(vertex);
        for (int i=0;i < it->second.size();++i)
        {
            vertices.push_back(it->second[i]);
        }
    }
    virtual void GetPrevVertices(int vertex, std::vector<int> &vertices) const override
    {
        for (auto it = list.begin();it != list.end();++it)
        {
            for (int i=0;i < it->second.size();++i)
            {
                if (it->second[i] == vertex)
                    vertices.push_back(it->first);
            }
        }
    }

    virtual void print () const override
    {
        for (auto it = list.begin();it != list.end();++it)
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
    std::vector<std::vector<int>> matrix;
public:
    MatrixGraph() {}
    MatrixGraph(const IGraph& _oth)
    {
        for (int i=1;i <= _oth.VerticesCount();++i)
        {
            std::vector<int> tmp;
            _oth.GetNextVertices(i, tmp);
            for (int j=0;j < tmp.size();++j)
            {
                AddEdge(i,tmp[j]);
            }

        }
    }
    virtual void AddEdge(int from, int to) override {
        if (from <= 0 || to <= 0)
            std::cout << "values must be greater than zero!" << std::endl;
        else {
            if (from > matrix.size() || to > matrix.size()) {
                if (from >= to) {
                    matrix.resize(from);
                    for (int i = 0; i < matrix.size(); ++i) {
                        matrix[i].resize(from);
                    }
                } else {
                    matrix.resize(to);
                    for (int i = 0; i < matrix.size(); ++i) {
                        matrix[i].resize(to);
                    }
                }
            }
            ++matrix[to - 1][from - 1];
        }
    }
    virtual int VerticesCount() const override
    {
        return matrix.size();
    }
    virtual void GetNextVertices(int vertex, std::vector<int> &vertices) const override
    {
        for (int i=0;i < matrix.size();++i)
        {
            if (matrix[i][vertex-1])
                for (int j=0;j < matrix[i][vertex-1];++j)
                    vertices.push_back(i+1);
        }
    }
    virtual void GetPrevVertices(int vertex, std::vector<int> &vertices) const override
    {
        for (int i=0;i < matrix.size();++i)
        {
            if (matrix[vertex-1][i])
            {
                for (int j=0;j < matrix[vertex-1][i];++j)
                    vertices.push_back(i+1);
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

    ListGraph g2 = g1;
    MatrixGraph g3(g2);

    g1.print();
    g2.print();
    g3.print();


    return 0;
}
