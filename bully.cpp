#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


#define MAX_NODES 5

typedef struct {
    int id;
    bool active;
    bool responded;
} Node;

typedef struct {
    Node nodes[MAX_NODES];
    int leader;
} BullyAlgorithm;

void initialize_nodes(BullyAlgorithm* bully) {
    for (int i = 0; i < MAX_NODES; i++) {
        bully->nodes[i].id = i + 1;
        bully->nodes[i].active = true;
        bully->nodes[i].responded = true;
    }
}

void start_election(BullyAlgorithm* bully, int initiating_node) {
    printf("Node %d initiating election.\n", initiating_node);

    for (int i = initiating_node; i < MAX_NODES; i++) {
        if (bully->nodes[i].active) {
            bully->nodes[i].responded = false;
            printf("Node %d sending election message to Node %d\n", initiating_node, bully->nodes[i].id);
        }
    }
}

void handle_election_message(BullyAlgorithm* bully, int sender_id) {
    printf("Node %d received election message from Node %d\n", bully->leader, sender_id);
    

    if (bully->leader == bully->nodes[sender_id - 1].id) {
        printf("Node %d is already the leader. Ignoring election message.\n", bully->leader);
    }
    else if (!bully->nodes[sender_id - 1].active) {
        printf("Node %d is inactive. Ignoring election message.\n", sender_id);
    }
    else {
        bully->leader = bully->nodes[sender_id - 1].id;
        printf("Node %d is the new leader.\n", bully->leader);
        

        // Send victory message to all other nodes
        printf("Node %d sending victory message to all nodes.\n", bully->leader);
        for (int i = 0; i < MAX_NODES; i++) {
            if (bully->nodes[i].active && bully->nodes[i].id != bully->leader) {
                printf("Node %d sending victory message to Node %d\n", bully->leader, bully->nodes[i].id);
            }
        }
    }
}

void handle_victory_message(BullyAlgorithm* bully, int sender_id) {
    printf("Node %d received victory message from Node %d\n", bully->leader, sender_id);
   

    bully->leader = bully->nodes[sender_id - 1].id;
    printf("Node %d updated leader to Node %d\n", bully->leader, bully->leader);
}

int main() {
    BullyAlgorithm bully;
    initialize_nodes(&bully);
    bully.leader = 0;

    // Simuler une défaillance du leader actuel (Node 5)
    bully.nodes[4].active = false;

    // Le nœud 3 détecte l'échec du leader et déclenche une élection
    start_election(&bully, 3);
    

    // Le nœud 2 reçoit le message d'élection du nœud 3 et se déclare lui-même comme leader
    handle_election_message(&bully, 3);
    

    // Le nœud 4 reçoit le message de victoire du nœud 2 et met à jour le leader actuel
    handle_victory_message(&bully, 2);

    return 0;
}
