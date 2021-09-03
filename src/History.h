#pragma once

#include "GameState.h"

// The data structure of History is a double linked list

struct Node
{
    GameState *gameState;
    struct Node *next;
    struct Node *prev;
};

typedef struct Node Node;

typedef struct
{
    Node *head;
    Node *curr;
} History;

Node* Node__construct();
void Node__destroy(Node *self);
History* History__construct();
void History__destroy(History *self);
bool History__empty(History *self);
bool History__overflow(History *self);
bool History__isNewest(History *self);
bool History__isOldest(History *self);
void History__write(History *self, const GameState *gameState);
GameState* History__undo(History *self);
GameState* History__redo(History *self);
void History__clear(History *self);