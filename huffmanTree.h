#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#define MAX_FILE_NAME_LEN 20    //Max length of file's name in arguments of main function.

typedef enum {FALSE, TRUE} Bool;    //boolean type
//struct to describe huffmanTreeNode
typedef struct
{
    int weight;
    char value;
    unsigned int parent;
    unsigned int lchild;
    unsigned int rchild;
    unsigned int size;    //all size of this node(include itself and its child)
} HuffmanTreeNode;

HuffmanTreeNode* initialHuffmanTree(int *size, char *huffmanTreeFile, char *characterSetFile);
void select(HuffmanTreeNode *huffmanArray, int boundary, int *minWeightIndex,
            int*secondMinWeightIndex);
Bool encode(HuffmanTreeNode *huffmanArray, int size, char *fileRes, char *fileDes);
Bool decode(HuffmanTreeNode *huffmanArray, int size, char *fileRes, char *fileDes);
Bool printCode(char *fileRes, char *fileDes);
Bool printHuffmanTree(HuffmanTreeNode *huffmanArray, int size, char *fileDes);
void printTree(HuffmanTreeNode *huffmanArray, int i, int size, int depth, FILE *fp);

#endif // HUFFMAN_TREE_H
