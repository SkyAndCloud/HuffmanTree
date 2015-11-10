/*Programmer: Yong Shan
 *Date: 2015/10/31
 *Describtion: Use Huffman algorithm to encode a file and decode a file
 *Else:
 */
#include <stdio.h>
#include <stdlib.h>      //malloc, free, exit
#include <string.h>
#include <conio.h>       //getch
#include "huffmanTree.h"

int main(int argc, char *argv[])
{
    char choice;    //choice of menu.
    char characterSetFile[MAX_FILE_NAME_LEN + 1] = "d:/characterSet.txt";
    char encodeFileRes[MAX_FILE_NAME_LEN + 1] = "d:/ToBeTran.txt";
    char huffmanTreeFile[MAX_FILE_NAME_LEN + 1] = "d:/hfmTree.txt";
    char encodeFileDes[MAX_FILE_NAME_LEN + 1] = "d:/CodeFile.txt";
    char decodeFileDes[MAX_FILE_NAME_LEN + 1] = "d:/TextFile.txt";
    char printCodeDes[MAX_FILE_NAME_LEN + 1] = "d:/CodePrin.txt";
    char printHuffmanTreeDes[MAX_FILE_NAME_LEN + 1] = "d:/TreePrint.txt";
    HuffmanTreeNode *huffmanTree = NULL;
    int characterSetSize;        //size of characters

    //if argc > 1, then read file'name from arguments.
    if (argc > 1)
    {
        switch (argc)
        {
        //a trick: update file's name by hiding "break;"
        default:
        case 8:
            strcpy(printHuffmanTreeDes, argv[7]);
        case 7:
            strcpy(printCodeDes, argv[6]);
        case 6:
            strcpy(decodeFileDes, argv[5]);
        case 5:
            strcpy(encodeFileDes, argv[4]);
        case 4:
            strcpy(huffmanTreeFile, argv[3]);
        case 3:
            strcpy(encodeFileRes, argv[2]);
        case 2:
            strcpy(characterSetFile, argv[1]);
            break;
        }
    }

    for ( ; ; )
    {
        system("cls");
        printf("****************Welcome to HuffmanCode!****************\n\n");
        printf("I : Initializing your HuffmanTree.\n");
        printf("E : Encode your file.\n");
        printf("D : Decode your file.\n");
        printf("P : Print your code file.\n");
        printf("T : Print your HuffmanTree.\n");
        printf("Q : Quit.\n\n");
        printf("Please input your choice: ");
        scanf(" %c", &choice);   //space before "%c"

        switch (choice)
        {
        case 'i':
        case 'I':
            huffmanTree = initialHuffmanTree(&characterSetSize, huffmanTreeFile, characterSetFile);
            break;
        case 'e':
        case 'E':
            encode(huffmanTree, characterSetSize, encodeFileRes, encodeFileDes);
            break;
        case 'd':
        case 'D':
            decode(huffmanTree, 2 * characterSetSize - 1, encodeFileDes, decodeFileDes);
            break;
        case 'p':
        case 'P':
            printCode(encodeFileDes, printCodeDes);
            break;
        case 't':
        case 'T':
            printHuffmanTree(huffmanTree, 2 * characterSetSize - 1, printHuffmanTreeDes);
            break;
        case 'q':
        case 'Q':
            printf("\n****************Thanks for your coming!****************\n");
            free(huffmanTree);
            exit(EXIT_SUCCESS);
        default:
            break;
        }
    }

    return 0;
}

