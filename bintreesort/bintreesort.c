/*
 * 二叉树排序法
 */
#include <stdlib.h>
#include <stdio.h>

struct BinTreeNode {
    int number;
    struct BinTreeNode* left;
    struct BinTreeNode* right;
};

/*分配空间*/
struct BinTreeNode* nodeAlloc(void) {
    return (struct BinTreeNode*)malloc(sizeof(struct BinTreeNode));
}

/*添加节点*/
struct BinTreeNode* addNode(struct BinTreeNode* p, int number) {
    if (p == NULL) {
        p = nodeAlloc();
        p->number = number;
        p->left = p->right = NULL;
    } else {
        if (number < p->number) {
            p->left = addNode(p->left, number);
        } else if (number > p->number) {
            p->right = addNode(p->right, number);
        }
    }
    return p;
}

/*释放二叉树*/
void freeBinTree(struct BinTreeNode* p) {
    if (p->left != NULL) {
        free(p->left);
    }
    if (p->right != NULL) {
        free(p->right);
    }
    free(p);
}

/*输出排序后的数组*/
void printSortedArray(struct BinTreeNode* root) {
    if (root == NULL) {
        return;
    }
    if (root->left != NULL) {
        printSortedArray(root->left);
    }
    printf(" %d", root->number);
    if (root->right != NULL) {
        printSortedArray(root->right);
    }
}

void SortedArray(struct BinTreeNode* root, int* n, int a[]) {
    if (root == NULL) {
        return;
    }
    if (root->left != NULL) {
        SortedArray(root->left, n, a);
    }
    a[*n] = root->number;
    (*n)++;
    if (root->right != NULL) {
        SortedArray(root->right, n, a);
    }
}

void printArray(int a[], int len) {
    int i;
    for (i = 0; i < len; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

int main() {
    int ia[] = {120, 88, 3, 1, 9, 2, 6, 4, 8, 10, 1, 77, 5};
    int len = sizeof(ia) / sizeof(ia[0]);
    struct BinTreeNode* root = NULL;
    int i;
    root = addNode(root, ia[0]);
    for (i = 1; i < len; i++) {
        addNode(root, ia[i]);
    }
    if (root == NULL) {
        printf("root==null");
    } else {
        int n = 0;
        printArray(ia, len);
        SortedArray(root, &n, ia);
        printArray(ia, n);
    }
    freeBinTree(root);
    return 0;
}
