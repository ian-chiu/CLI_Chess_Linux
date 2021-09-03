#include <stdlib.h>
#include <assert.h>
#include "History.h"

Node* Node__construct()
{
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->gameState = NULL;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

void Node__destroy(Node *self)
{
    free(self->gameState);
    free(self);
    self = NULL;
}

static void History__init(History *self)
{
    self->curr = NULL;
    self->head = NULL;
}

History* History__construct()
{
    History *result = (History*)malloc(sizeof(History));
    History__init(result);
    return result;
}

void History__destroy(History *self)
{
    Node *curr = self->head;
    while (curr) {
        Node* temp = curr->next;
        free(curr);
        curr = temp;
    }
    self = NULL;
}

bool History__empty(History *self)
{
    return self->head == NULL;
}

bool History__isNewest(History *self)
{
    return self->head != NULL && self->curr->next == NULL;
}

bool History__isOldest(History *self)
{
    assert(!History__empty(self));
    return self->curr == self->head;
}

void History__write(History *self, const GameState *gameState)
{
    Node *newNode = Node__construct();
    newNode->gameState = GameState__copyConstruct(gameState);
    if (History__isNewest(self)) {
        // if the current node of the history is the latest we directly connect the new game state node to 
        // the next of the current node and set the current node to the new game state node
        newNode->prev = self->curr;
        newNode->next = NULL;
        self->curr->next = newNode;
        self->curr = self->curr->next;
    }
    else if (self->head == NULL) {
        // if the history is empty, we add the new game state node directly to the history
        // and set the history's head and curr to the new game state node
        self->head = newNode;
        self->curr = self->head;
    }
    else {
        // if the current node of the history is not the latest (player has undone the game), we set the new game state
        // to the next node of the current node and set the current node to its next. Moreover, we destroy all nodes next 
        // to the current node, because it is now the newest history node after rewriting the history.
        self->curr->next->gameState = GameState__copyConstruct(gameState);
        self->curr = self->curr->next;
        Node *curr = self->curr->next;
        self->curr->next = NULL;
        while (curr) {
            Node *temp = curr->next;
            Node__destroy(curr);
            curr = temp;
        }
    }
}

GameState* History__undo(History *self)
{
    assert(!History__isOldest(self));
    // change current state to previous state using prev pointer of the double liinked list
    self->curr = self->curr->prev;
    // restore and return the previous game state
    GameState *lastGameState = GameState__copyConstruct(self->curr->gameState);
    return lastGameState;
}

GameState* History__redo(History *self)
{
    assert(!History__isNewest(self));
    // change current state to the next state using next pointer of the double liinked list
    self->curr = self->curr->next;
    // restore and return the next game state
    GameState *nextGameState = GameState__copyConstruct(self->curr->gameState);
    return nextGameState;
}

void History__clear(History *self)
{
    History__init(self);
}