#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

class Vertex {
public:
    std::vector<int> adjacency_vector;
    int initial_number;
    int degree;

    Vertex() : adjacency_vector(), initial_number(0), degree(0) {}
};

class Graph {
private:
    Vertex *vertices;
    unsigned int countVertices;
public:
    Graph() : vertices() {}

    Graph(std::string s) : Graph() {
        std::ifstream infile;
        infile.open(s.c_str(), std::ios::out);
        int v1, v2;

        if(infile >> countVertices) {
            vertices=new Vertex[countVertices];

            for(unsigned int i=0; i<countVertices; i++) {
                vertices[i].initial_number=i;
                vertices[i].adjacency_vector.resize(countVertices);
            }
        } else {
            std::cout<<"failed to read"<<std::endl;
        }

        while (infile >> v1 >> v2) {
            vertices[v1].adjacency_vector[v2]=1;
            vertices[v2].adjacency_vector[v1]=1;
            vertices[v1].degree++;

            if(v1!=v2) {
                vertices[v2].degree++;
            }
        }
    }

    ~Graph() {
        delete[] vertices;
    }

    int amount_of_vertices() const {
        return countVertices;
    }

    int amount_of_edges() const {
        int counter=0;

        for(unsigned int i=0; i<countVertices; i++) {
            for(unsigned int j=i; j<countVertices; j++) {
                if(vertices[i].adjacency_vector[j]==1) {
                    counter++;
                }
            }
        }

        return counter;
    }

    Vertex *get_vertices_array() const {
        return vertices;
    }

    int *create_degree_array() const {
        int *degree_array=new int[countVertices];

        for(unsigned int i=0; i<countVertices; i++) {
            degree_array[i]=0;
        }

        for(unsigned int i=0; i<countVertices; i++) {
            for(unsigned int j=0; j<countVertices; j++) {
                if(vertices[i].adjacency_vector[j]==1) {
                    degree_array[i]++;
                }
            }
        }

        std::sort(degree_array, degree_array+countVertices);
        return degree_array;
    }

    void permute_adjacency_matrix(std::vector<Vertex> &arr, int i, int n, bool &iso) {
        int j;
        iso=true;

        for(unsigned int j=0; j<countVertices && iso!=false; j++) {
            for(unsigned int k=0; k<countVertices; k++) {
                if(vertices[j].adjacency_vector[k]!=arr[j].adjacency_vector[k]) {
                    iso=false;
                    break;
                }
            }
        }

        if (i==n || iso==true) {
            return;
        } else {
            for (j = i; j < n && iso!=true; j++) {
                std::swap(arr[i], arr[j]);

                for(int k=0; k<n; k++) {
                    std::swap(arr[k].adjacency_vector[i], arr[k].adjacency_vector[j]);
                }

                permute_adjacency_matrix(arr, i+1, n, iso);

                if(iso!=true) {
                    for(int k=0; k<n; k++) {
                        std::swap(arr[k].adjacency_vector[i], arr[k].adjacency_vector[j]);
                    }

                    std::swap(arr[i], arr[j]);
                }
            }
        }
    }

    void print_adjacency_matrix() {
        for(unsigned int i=0; i<countVertices; i++) {
            for(unsigned int j=0; j<countVertices; j++) {
                std::cout<<vertices[i].adjacency_vector[j]<<" ";
            }

            std::cout<<std::endl;
        }
    }

    bool operator==(const Graph &cmp) {
        if(amount_of_vertices()!=cmp.amount_of_vertices() || amount_of_edges()!=cmp.amount_of_edges()) {
            return false;
        }

        int *this_degree_array=create_degree_array();
        int *cmp_degree_array=cmp.create_degree_array();

        for(unsigned int i=0; i<countVertices; i++) {
            if(this_degree_array[i]!=cmp_degree_array[i]) {
                return false;
            }
        }

        std::vector<Vertex> tmp(countVertices);

        for(unsigned int i=0; i<countVertices; i++) {
            tmp[i].adjacency_vector=cmp.get_vertices_array()[i].adjacency_vector;
            tmp[i].degree=cmp.get_vertices_array()[i].degree;
            tmp[i].initial_number=cmp.get_vertices_array()[i].initial_number;
        }

        bool isomorphic=false;

        permute_adjacency_matrix(tmp, 0, countVertices, isomorphic);

        if(isomorphic==true) {
            for(unsigned int i=0; i<countVertices; i++) {
                std::cout<<vertices[i].initial_number<<"->"<<tmp[i].initial_number<<std::endl;
            }
        }

        return isomorphic;
    }
};

int main(int argc, char **argv) {
    Graph g1(argv[1]);
    g1.print_adjacency_matrix();
    std::cout<<"g1.amount_of_vertices()="<<g1.amount_of_vertices()<<"\ng1.amount_of_edges()="<<g1.amount_of_edges()<<std::endl;
    int *arr1=g1.create_degree_array();

    for(int i=0; i<g1.amount_of_vertices(); i++) {
        std::cout<<arr1[i]<<" ";
    }
    std::cout<<std::endl;
    std::cout<<std::endl;

    Graph g2(argv[2]);
    g2.print_adjacency_matrix();
    std::cout<<"g2.amount_of_vertices()="<<g2.amount_of_vertices()<<"\ng2.amount_of_edges()="<<g2.amount_of_edges()<<std::endl;
    int *arr2=g2.create_degree_array();
    for(int i=0; i<g2.amount_of_vertices(); i++) {
        std::cout<<arr2[i]<<" ";
    }
    std::cout<<std::endl;
    std::cout<<std::endl;

    if(g1==g2) {
        std::cout<<"Isomorphic"<<std::endl;
    } else {
        std::cout<<"Not Isomorphic"<<std::endl;
    }
    return 0;
}
