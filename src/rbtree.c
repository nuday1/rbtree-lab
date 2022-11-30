#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nilNode = (node_t *)calloc(1, sizeof(node_t));
  
  nilNode->color = RBTREE_BLACK;
  p->root = nilNode;
  p->nil = nilNode; 
  return p;
}

void free_node(rbtree *t, node_t *x) {
  //후위 순회로 각 노드의 메모리 반환
  if (x->left != t->nil)
    free_node(t, x->left);
  if (x->right != t->nil)
    free_node(t, x->right);
  free(x);
  x = NULL;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t->root != t->nil)
    free_node(t, t->root); //루트부터 시작하여 노드 제거
  free(t->nil); //nil 제거
  free(t); //트리 제거
}

void left_rotate(rbtree *t, node_t *x){
  //x->right가 nil이 아님을 가정한다
  node_t *y; //y를 가리키는 포인터

  y = x->right;
  x->right = y->left;

  if (y->left != t->nil)
    y->left->parent = x;

  y->parent = x->parent;

  if (x->parent == t->nil) //x가 루트이면
    t->root = y;
  else if (x == x->parent->left) //x가 x.p의 왼쪽 자식
    x->parent->left = y;
  else
    x->parent->right = y;

  y->left = x;
  x->parent = y;  
}

void right_rotate(rbtree *t, node_t *x){
  //x->left가 nil이 아님을 가정한다
  node_t *y; //y를 가리키는 포인터

  y = x->left;
  x->left = y->right;

  if (y->right != t->nil)
    y->right->parent = x;

  y->parent = x->parent;

  if (x->parent == t->nil) //x가 루트이면
    t->root = y;
  else if (x == x->parent->left) //x가 x.p의 왼쪽 자식
    x->parent->left = y;
  else
    x->parent->right = y;

  y->right = x;
  x->parent = y;  
}

void rbtree_insert_fixup(rbtree *t, node_t *z) {
  node_t *y;
  while (z->parent->color == RBTREE_RED) {// z의 부모가 RED
    // z의 부모가 z 조부모의 왼쪽 서브트리
    if (z->parent == z->parent->parent->left) {
      //삼촌은 조부모의 오른쪽
      y = z->parent->parent->right;
      // case 1: z의 삼촌이 RED일 경우
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent; //z가 조부모? 조부모가 z?
      }
      // case 2: z의 삼촌 y가 BLACK이고, z가 오른쪽 자식
      else {
        if (z == z->parent->right) {
          z = z->parent;
          left_rotate(t, z);
      }
      // case 3: z의 삼촌 y가 BLACK이고, z가 왼쪽 자식
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }
    //z의 부모가 z 조무보의 오른쪽 서브트리
    else {
      // 삼촌은 조부모의 왼쪽
      y = z->parent->parent->left;
      // case 4: z의 삼촌 y가 RED
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent; //z가 조부모? 조부모가 z?
      }
      else {
        // case 5: z의 삼촌 y가 BLACK, z가 왼쪽 자식
        if (z == z->parent->left) {
          z = z->parent;
          right_rotate(t, z);
        }
        // case 6: z의 삼촌 y가 BLACK, z가 오른쪽 자식
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *x = t->root;
  node_t *y = t->nil;
  node_t *z = (node_t *)calloc(1, sizeof(node_t));

  z->key = key;

  while (x != t->nil) { // 만약 현재 아무 노드가 없으면 *x == *y == nilNode인가?
    y = x;
    if (z->key < x->key)
      x = x->left;
    else
      x = x->right;
  }
  z->parent = y;

  if (y == t->nil)
    t->root = z;
  else if (z->key < y->key)
    y->left = z;
  else
    y->right = z;

  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  rbtree_insert_fixup(t, z);
  return z; // 리턴값의 기준? 
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *ans = t->root;
  
  while (ans != t->nil) {
    if (ans->key == key)
      return ans;

    if (ans->key < key){
      ans = ans->right;
    }
    else {
      ans = ans->left;
    }
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  //트리가 비어있을 경우
  if (t->root == t->nil) {
    return NULL;
  }

  node_t *ans = t->root;

  while (ans->left != t->nil) {
    ans = ans->left;
  }

  return ans; // 이렇게 하면 오답인가?!
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  if (t->root == t->nil) {
    return NULL;
  }

  node_t *ans = t->root;

  while (ans->right != t->nil) {
    ans = ans->right;
  }

  return ans;
}

//서브트리의 succesor 찾는 함수
node_t* subtree_min(const rbtree *t, node_t *z){
  
  while (z->left != t->nil) {
    z = z->left;
  }

  return z;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v) {
  if (u->parent == t->nil)
    t->root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  v->parent = u->parent;
}

void rbtree_erase_fixup(rbtree *t, node_t *x) {
  while (x != t->root && x->color == RBTREE_BLACK) {
    if (x == x->parent->left) {
      node_t *w = x->parent->right;
      // case 1: x의 형제 w가 RED
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      // case 2: x의 형제 w가 BLACK이고 w의 두 자식이 모두 BLACK
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      // case 3: x의 형제 w는 BLACK, w의 왼쪽 자식은 RED, W의 오른쪽 자식은 BLACK
      else {
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        // case 4: x의 형제 w는 BLACK, w의 오른쪽 자식은 RED
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }
    else {
      node_t *w = x->parent->left;
      // case 5: x의 형제 w가 RED
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      // case 6: x의 형제 w가 BLACK이고 w의 두 자식이 모두 BLACK
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      // case 7: x의 형제 w는 BLACK, w의 왼쪽 자식은 BLACK, W의 오른쪽 자식은 RED
      else {
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        // case 8: x의 형제 w는 BLACK, w의 왼쪽 자식은 RED
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
  node_t *y = z;
  node_t *x;
  color_t yOriginalColor = y->color;

  if (z->left == t->nil) {
    x = z->right;
    rbtree_transplant(t,z,z->right);
  }
  else if (z->right == t->nil) {
    x = z->left;
    rbtree_transplant(t,z,z->left);
  }
  else {
    y = subtree_min(t,z->right);
    yOriginalColor = y->color;
    x = y->right;

    if (y->parent == z)
      x->parent = y;
    else {
      rbtree_transplant(t,y,y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    rbtree_transplant(t,z,y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  if (yOriginalColor == RBTREE_BLACK)
    rbtree_erase_fixup(t,x);
  key_t ans = z->key;
  free(z);
  return ans;
}
void subtree_to_array(const rbtree *t, node_t *curr, key_t *arr, size_t n, size_t *count) {
  if (curr == t->nil) {
    return;
  }
  subtree_to_array(t, curr->left, arr, n, count);
  if (*count < n) {
    arr[(*count)++] = curr ->key;
  }
  else return;
  subtree_to_array(t, curr->right, arr, n, count);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  if (t->root == t->nil) {
    return 0;
  }
  size_t cnt = 0;
  subtree_to_array(t, t->root, arr, n, &cnt);
  return 0;
}
