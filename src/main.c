/*******************************************************************************
 * Copyright (c) 2021, Initialsoft, Inc.
 * All rights reserved.
 *
 * main : [description]
 *
 * filename : main.cc
 * author   : hunt978(hunt978@initialsoft.com)
 * create   : 2021-03-15 03:15:35 UTC
 * modified : 2021-03-17 16:24:15 UTC
\******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// Headers
//
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constants
//
#define nullptr NULL

////////////////////////////////////////////////////////////////////////////////
// Inner Scope Objects
//
struct Node {
  uint32_t fid;
  uint32_t tid;

  struct Node* prev;
  struct Node* next;
};

/*  */

/**
 * @brief      insert a node into list
 *
 *             Given a reference (pointer to pointer) to the head of a
 *             list, inserts a new node on the front of the
 *             list with given fid and tid.
 *
 * @param[in out]    head_ref  head reference
 * @param[in]  fid   The fid
 * @param[in]  tid   The tid
 *
 * @return     void
 */
void push(struct Node** head_ref, uint32_t fid, uint32_t tid)
{
    // allocate node
    struct Node* new_node
        = (struct Node*)malloc(sizeof(struct Node));

    // put in the data
    new_node->fid = fid;
    new_node->tid = tid;

    // Make next of new node as head and previous as NULL
    new_node->next = (*head_ref);
    new_node->prev = nullptr;

    // change prev of head node to new node
    if ((*head_ref) != nullptr)
        (*head_ref)->prev = new_node;

    // move the head to point to the new node
    (*head_ref) = new_node;
}

/*  */
/**
 * @brief      insert after
 *
 *             Given a node as prev_node, insert a new node after the
 *             given node
 *
 * @param[in]  prev node
 * @param[in]  fid   The fid
 * @param[in]  tid   The tid
 *
 * @return     void
 */
void insertAfter(struct Node* prev_node, uint32_t fid, uint32_t tid)
{
    // check if the given prev_node is NULL
    if (prev_node == nullptr) {
        return;
    }

    // allocate new node
    struct Node* new_node
        = (struct Node*)malloc(sizeof(struct Node));

    // put in the data
    new_node->fid = fid;
    new_node->tid = tid;

    // Make next of new node as next of prev_node
    new_node->next = prev_node->next;

    // Make the next of prev_node as new_node
    prev_node->next = new_node;

    // Make prev_node as previous of new_node
    new_node->prev = prev_node;

    // Change previous of new_node's next node
    if (new_node->next != nullptr)
        new_node->next->prev = new_node;
}


/**
 * @brief      insert a node into list
 *
 *             there are 4 strains for the container :
 *
 *               1) from id and to id **CANNOT** be the same
 *               2) it should **NOT** hold duplicated node
 *               3) it should **NOT** hold circled link
 *               4) fan-out is allowed while fan-in is **NOT**.
 *
 * @param      head  The head
 * @param[in]  fid   The fid
 * @param[in]  tid   The tid
 *
 * @return     new head
 */
struct Node* insert_node(struct Node* head, uint32_t fid, uint32_t tid)
{
    // Rule1 no equeal
    if (fid == tid)
        return head;

    struct Node *index = head;
    struct Node* prev_node = nullptr;
    while (index) {
        // Rule2 no repeat
        if (index->fid == fid && index->tid == tid)
            return head;

        // Rule3 no closed loop
        if (tid == index->fid)
            return head;

        // Rule4 Fan-out is allowed, fan-in is prohibited
        // tid should not appear twice.
        if (tid == index->tid)
            return head;

        if (!prev_node && (index->fid < fid || (index->fid == fid && index->tid < tid)))
            prev_node = index;

        index = index->next;
    }

    // insert node in order.
    if (prev_node) {
        insertAfter(prev_node, fid, tid);
    } else {
        push(&head, fid, tid);
    }

    return head;
}

/**
 * @brief      remove a node from list
 *
 * @param      head  The head
 * @param[in]  fid   The fid
 * @param[in]  tid   The tid
 *
 * @return     new head
 */
struct Node* remove_node(struct Node* head, uint32_t fid, uint32_t tid)
{
    struct Node *index = head;
    while (index) {
        if (index->fid == fid && index->tid == tid)
            break;

        index = index->next;
    }

    struct Node *del = index;

    if (del == nullptr)
        return nullptr;

    // If node to be deleted is head node
    if (head == del)
        head = del->next;

    // Change next only if node to be deleted is NOT the last node
    if (del->next != nullptr)
        del->next->prev = del->prev;

    // Change prev only if node to be deleted is NOT the first node
    if (del->prev != nullptr)
        del->prev->next = del->next;

    // Finally, free the memory occupied by del
    free(del);
    return head;
}

/**
 * @brief      Dumps a list.
 *
 * @param      head  The head
 */
void dump_list(struct Node* head)
{
  printf("---------------------------\n");
  struct Node* node = head;
  while (node) {
    printf("  %d -> %d\n", node->fid, node->tid);
    node = node->next;
  }
}

/**
 * @brief      Does a test.
 */
void do_test()
{
  struct Node* head = nullptr;

  head = insert_node(head, 1, 2);
  dump_list(head);
  head = insert_node(head, 2, 4);
  dump_list(head);
  head = insert_node(head, 2, 3);
  dump_list(head);
  head = insert_node(head, 3, 4);
  dump_list(head);
  head = insert_node(head, 4, 1);
  dump_list(head);
  head = insert_node(head, 1, 2);
  dump_list(head);

  while (head) {
    head = remove_node(head, head->fid, head->tid);
  }
}

////////////////////////////////////////////////////////////////////////////////
// Classes
//

////////////////////////////////////////////////////////////////////////////////
// Functions
//
int main(int argc, char const* argv[])
{
  do_test();

  return 0;
}

////////////////////////////////// EOF /////////////////////////////////////////
