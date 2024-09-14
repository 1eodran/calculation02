#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void createQ(Queue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
}
void enQ(Queue* queue, char opt, double opd) {
    Node* tmp = (Node*)malloc(sizeof(Node));

    tmp->chData = opt;
    tmp->dbData = opd;

    tmp->next = NULL;

    if (isemptyQ(queue)) {
        queue->front = tmp;
        queue->rear = tmp;
        return;
    }
    queue->rear->next = tmp;
    queue->rear = tmp;
}

double dbdeQ(Queue* queue) {
    if (isemptyQ(queue)) {
        return 0;
    }
    Node* tmp = queue->front;
    double frontData = tmp->dbData;
    queue->front = queue->front->next;
    free(tmp);
    return frontData;
}

char chdeQ(Queue* queue) {
    if (isemptyQ(queue)) {
        return 0;
    }
    Node* tmp = queue->front;
    char frontData = tmp->chData;
    queue->front = queue->front->next;
    free(tmp);
    return frontData;
}


char frontQ(Queue* queue) {
    if (isemptyQ(queue)) {
        return 0;
    }
    return queue->front->chData;
}

int isemptyQ(Queue* queue) {
    return (queue->front == NULL);
}

void destroyQ(Queue* queue) {
    while (!isemptyQ(queue)) {
        chdeQ(queue);
        dbdeQ(queue);
    }
}