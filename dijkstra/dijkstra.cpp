/* 
Dijkstra é um dos algoritmos que calcula o caminho de custo mínimo entre vértices de um grafo. Escolhido um vértice como raiz da busca, este algoritmo calcula o custo mínimo deste vértice para todos os demais vértices do grafo
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
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

void dijkstra(Grafo* grafo, int* distancia, int* predecessor, int start);
int encontramindistancia(int* distancia, int* visitado, int gSize);
void printPath(ostream* output, int initial, int final, int gSize, int* predecessor);

void dijkstra(Grafo* grafo, int* distancia, int* predecessor, int start) {
  int gSize, contador, vertice, adjj;
  gSize = grafo->getSize();
  contador = gSize;
  int visitado[gSize];
  
  for (int i = 0; i < gSize; i++) {
    predecessor[i] = -1;
    distancia[i] = -1;
    visitado[i] = 0;
  }

  distancia[start] = 0;
  while(contador > 0) {
    vertice = encontramindistancia(distancia, visitado, gSize);
    if(vertice == -1) {
      break;
    }
    visitado[vertice] = 1;
    contador--;
    Node* currentNode = grafo->getHead()[vertice];        
    while (currentNode != nullptr) {
      adjj = currentNode->valor;
      if(distancia[adjj] < 0) {
        distancia[adjj] = distancia[vertice] + currentNode->custo;
        predecessor[adjj] = vertice;
      } else {
        if(distancia[adjj] > distancia[vertice] + currentNode->custo) {
          distancia[adjj] = distancia[vertice] + currentNode->custo;
          predecessor[adjj] = vertice;
        }
      }

      currentNode = currentNode->next;
    }
  }      
}

int encontramindistancia(int* distancia, int* visitado, int gSize) {
  int mindistancia = -1;
  bool first = true;
  for(int i = 0; i < gSize; i++) {
    if(distancia[i] >= 0 && visitado[i] == 0) {
      if(first) {
        mindistancia = i;
        first = false;
      } else {
        if(distancia[mindistancia] > distancia[i]) {
          mindistancia = i;
        }
      }
    }
  }

  return mindistancia;
}

void printPath(ostream* output, int initial, int final, int gSize, int* predecessor) {
  bool flag = false;
  int contador = 0;
  int path[gSize];
  for(int i = 0; i < gSize; i++) {
    path[i] = -1;
  } 
  int aux = final;
  while(aux != -1) {
    if(aux < 0) {        
      flag = true;
      break;
    }    
    path[contador] = aux + 1;
    aux = predecessor[aux];
    contador++;
  } 
  for(int i = contador; i >= 0; i--) {
    if(path[i] != -1) {
      *output << path[i];
      if(path[i-1] != -1 && i > 0) {
        *output << " -> ";
      }
    }
  }  
}

int main(int argc, char** argv) {
  Argument args;
  bool sucess = get_arguments(argc, argv, &args);
  if (sucess) {
    if (args.help) {
      help_arguments("O algoritmo de Dijkstra é um algoritmo para encontrar os caminhos mais curtos entre os nós em um grafo.");
    } else {
      if(args.initial != -1) {
        Grafo grafo = criargrafo(args.input);
        int gSize = grafo.getSize();
        int distancia[gSize];
        int predecessor[gSize];

        dijkstra(&grafo, distancia, predecessor, args.initial - 1);        
        if(args.final != -1) {
          if(distancia[args.final - 1] != -1) {
            if(args.solution) {
              printPath(args.output, args.initial - 1, args.final - 1, gSize, predecessor);
              *args.output << endl;
              *args.output << distancia[args.final - 1] << endl;
            } else {
              *args.output << distancia[args.final - 1] << endl;
            }    
          } else {
            *args.output << "Não há um caminho" << args.initial << " - " << args.final << endl;
          }
        } else {
            if(args.solution) {
              for (int i = 0; i < gSize; i++) {
                printPath(args.output, args.initial - 1, i, gSize, predecessor);                
                *args.output << " : " << distancia[i] << endl;
              }
            } else {
              for (int i = 0; i < gSize; i++) {
                *args.output << i+1 << ":" << distancia[i] << " ";
              }
              *args.output << endl;
            }
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
