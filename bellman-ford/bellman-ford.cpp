/*
Bellman-Ford é um algoritmo que calcula os caminhos mais curtos de um único vértice de origem para todos os outros vértices em um dígrafo ponderado .
*/

#include <iostream>
#include <sstream>
#include <limits.h>
#include <bits/stdc++.h>
#include "../utils/utils.h"

using namespace std;

typedef struct Path {
    int src;
    int dest;
    int weight;
}path;

typedef struct Graph {
    int Vert;
    int Edges;
    Path* path;
}graph;
  
graph* createGraph(int V, int E);

void printArr(int dist[], int n);

void BellmanFord(struct Graph* graph, int src);

graph* createGraph(int V, int E) 
{ 
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->Vert = V; 
    graph-> Edges = E;
    graph -> path = (Path*)malloc(graph -> Edges * sizeof(Path));
    return graph; 
} 

void printArr(int dist[], int n) 
{ 
    printf("Vertice   Distancia da origem\n");
    for (int i = 0; i < n; ++i)
        printf("%d \t\t %d\n", i, dist[i]);
} 

void BellmanFord(struct Graph* graph, int src) 
{ 
    int V = graph->Vert; 
    int E = graph->Edges; 
    int dist[V]; 

    for (int i = 0; i < V; i++) 
        dist[i] = INT_MAX; 
    dist[src] = 0; 

    for (int i = 1; i <= V - 1; i++) { 
        for (int j = 0; j < E; j++) { 
            int u = graph->path[j].src; 
            int v = graph->path[j].dest; 
            int weight = graph->path[j].weight; 
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) 
                dist[v] = dist[u] + weight; 
        } 
    } 
  

    for (int i = 0; i < E; i++) { 
        int u = graph->path[i].src; 
        int v = graph->path[i].dest; 
        int weight = graph->path[i].weight; 
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) { 
           printf("Graph contains negative weight cycle\n"); 
            return;
        } 
    } 
  
    printArr(dist, V);
  
    return; 
}

int main(){
    
    int V, E; 
    scanf("%d %d", &V, &E);
    Graph* graphs = createGraph(V, E);
    int i, src, dest, weight;
    for(i = 0; i < E; i++){
        scanf("%d %d %d", &src, &dest, &weight);
        graphs->path[i].src = src;
        graphs->path[i].dest = dest;
        graphs->path[i].weight = weight;
    }

    BellmanFord(graphs, 0);

    return 0;
}
