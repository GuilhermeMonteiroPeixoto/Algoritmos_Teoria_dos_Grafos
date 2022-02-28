#include <iostream>
#include <sstream>
#include <limits.h>
using namespace std;

#include "../utils/utils.h"

struct Node {
  int valor, custo;
  Node* next;
};

struct Link {
  int src, dest, peso;
};

class Grafo {
  private:
    int size;
    Node** head;
  public:
    Grafo(Link links[], int e_contador, int v_contador);
    ~Grafo();
    int getSize() { return this->size; };
    Node** getHead() { return this->head; };
    Node* addNode(int valor, int peso, Node* head);
    void display(ostream* output);
};

Grafo criargrafo(istream* input, bool undirected = false);
Grafo::Grafo(Link links[], int e_contador, int v_contador) {
  this->head = new Node*[v_contador]();
  this->size = v_contador;
  for (int i = 0; i < v_contador; ++i) {
    head[i] = nullptr;
  }
  for (unsigned i = 0; i < e_contador; i++) {
    int src_ver = links[i].src;
    int dest_ver = links[i].dest;
    int peso_ver = links[i].peso;
    
    Node* new_node = addNode(dest_ver, peso_ver, this->head[src_ver]);
    this->head[src_ver] = new_node;
  }
}

Grafo::~Grafo() {
  for (int i = 0; i < size; i++) {
    delete[] head[i];
  }
  delete[] head;
}

Node* Grafo::addNode(int valor, int peso, Node* head) {
  Node* new_node = new Node;
  new_node->valor = valor;
  new_node->custo = peso;
  new_node->next = nullptr;
  if (head != nullptr) {
    if (head->custo > new_node->custo) {
      new_node->next = head;
    } else {
      Node* prev_node = head;
      Node* current_node = prev_node->next;

      while (current_node != nullptr && (current_node->custo < new_node->custo)) {
        prev_node = current_node;
        current_node = current_node->next;
      }
      prev_node->next = new_node;
      new_node->next = current_node;

      return head;
    }
  }

  return new_node;
}

void Grafo::display(ostream* output) {
  for (int i = 0; i < this->size; i++) {
    Node* currentNode = this->head[i];
    while (currentNode != nullptr) {
      *output << "(" << i + 1 << ", " << currentNode->valor + 1 << ", " << currentNode->custo << ") ";
      currentNode = currentNode->next;
    }   
    if (this->head[i] != nullptr) {
      *output << endl;
    }
  }
}

Grafo criargrafo(istream* input, bool undirected) {
  int v_contador, e_contador;
  
  *input >> v_contador >> e_contador;
  if (undirected) {
    e_contador *= 2;
  }
  string line;
  getline(*input, line);
  Link links[e_contador] = {};
  for (int i = 0; i < e_contador; i += undirected ? 2 : 1) {
    getline(*input, line);  
    istringstream line_cin(line);
    int src, dest, peso = 1;
    line_cin >> src >> dest >> peso;
    links[i] = {src - 1, dest - 1, peso};
    if (undirected) {
      links[i + 1] = {dest - 1, src - 1, peso};
    }
  }

  return Grafo(links, e_contador, v_contador);
}

int** solucaografo(int size);

int** solucaografo(int size) {
  int** grafo = new int * [size];
  for (int i = 0; i < size; i++) {
    grafo[i] = new int[size];
    for (int j = 0; j < size; j++) {
      grafo[i][j] = -INT_MAX;
    }
  }
  return grafo;
}

int prim(Grafo* grafo, int src, bool solution, ostream* output);

int prim(Grafo* grafo, int src, bool solution, ostream* output) {
  int custo = 0;
  int v_contador = grafo->getSize();
  int visited[v_contador] = {};
  int n_visited = 1;
  visited[src] = 1;
  int** solution_list = solucaografo(v_contador);
  
  while (n_visited < v_contador) {
    int current_min = INT_MAX;
    int pos_i, pos_j = 0;
    for (int i = 0; i < v_contador; i++) {
      if (visited[i]) {
        Node* current_node = grafo->getHead()[i];
        while (current_node != nullptr) {
          int j = current_node->valor;
          if (!visited[j]) {
            int curret_custo = current_node->custo;
            if (curret_custo < current_min) {
              current_min = curret_custo;
              pos_i = i;
              pos_j = j;
            }
          }
          current_node = current_node->next;
        }
      }
    }

    visited[pos_j] = current_min;
    n_visited += 1;
    solution_list[pos_i][pos_j] = current_min;
    custo += current_min;
  }

  if (solution) {
    for (int i = 0; i < v_contador; i++) {
      for (int j = 0; j < v_contador; j++) {
        if (solution_list[i][j] > -INT_MAX) {
          *output << "(" << i + 1 << "," << j + 1 << ") ";
        }
      }
    }
    *output << endl;
  }
  return custo;
}

int main(int argc, char** argv) {
  Argument args;
  bool sucess = get_arguments(argc, argv, &args);
  if (sucess) {
    if (args.help) {
      help_arguments(" o algoritmo de Prim é um algoritmo guloso (greedy algorithm) empregado para encontrar uma árvore geradora mínima (minimal spanning tree) num grafo conectado, valorado e não direcionado.", true, false);
    } else {
      if (args.initial != -1) {
        Grafo grafo = criargrafo(args.input, true);
        int custo = prim(&grafo, args.initial - 1, args.solution, args.output);        
        if (!args.solution) {
          *args.output << custo << endl;
        }
      } else {
        cout << "Argumento de vértice inicial ausente!" << endl;
      }
    }
  } else {
    cout << "Os argumentos inseridos são inválidos!" << endl;
  }
  return 0;
}
