#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "huffmanTree.h"

/*Name: initialHuffmanTree
 *Interface:
 *    Arguments:
 *        int *size             :   change characterSet's size using pointer
 *        char *huffmanTreeFile :   huffmanTreeFile's dir and name
 *        char *characterSetFile:   characterSetFile's dir and name
 *    Return : huffman tree array's pointer, and outer pointer will point to it.
 *Use: initialize huffmanArray by reading data from characterSetFile,
 *     generate hole tree and save to huffmanTreeFile
 *Else: ALL ERRORS WILL PRINT LOGS!
 *      if error occurs when opening file, then exit(EXIT_FAILURE);
 *      if error occurs when reading data, then return huffmanArray;
 *      if error occurs when allocating memory, then return huffmanArray;
 */
HuffmanTreeNode* initialHuffmanTree(int *size, char *huffmanTreeFile, char *characterSetFile)
{
    int characterSetSize = 0;
    int huffmanArraySize;
    char *characterSet;   //imitate array with changeable length
    int *characterSetWeight;  //imitate array with changeable length
    int min1;   //the index with min weight
    int min2;   //the index with secongMin weight
    int i;      //counter
    HuffmanTreeNode *p;
    HuffmanTreeNode *huffmanArray;  //imitate array with changeable length
    FILE *fp;

    if ((fp = fopen(characterSetFile, "rb")) == NULL)
    {
        fprintf(stderr, "\nError caused by opening characterSetFile!"
                "Press any key to exit...\n");
        getch();
        exit(EXIT_FAILURE);
    }
    rewind(fp);
    //read characterSetSize from characterSetFile
    if (fscanf(fp, "%d", &characterSetSize) != 1)
    {
        fprintf(stderr, "\nError caused by reading cahracterSetSize in characterSetFile!"
                "Press Any key to exit...\n");
        getch();
        exit(EXIT_FAILURE);
    }
    *size = characterSetSize;
    huffmanArraySize = 2 * characterSetSize - 1;
    if (characterSetSize > 0)
    {
        if ((huffmanArray = (HuffmanTreeNode *)malloc((huffmanArraySize + 1)
                            * sizeof(HuffmanTreeNode))) == NULL)
        {
            fprintf(stderr, "\nNo memory available!\n");
            getch();
            return huffmanArray;
        }
        if ((characterSet = (char *)malloc(characterSetSize * sizeof(char))) == NULL)
        {
            fprintf(stderr, "\nNo memory available!\n");
            getch();
            return huffmanArray;
        }
        if ((characterSetWeight = (int *)malloc(characterSetSize * sizeof(int))) == NULL)
        {
            fprintf(stderr, "\nNo memory available!\n");
            getch();
            return huffmanArray;
        }
        for (i = 0; i < characterSetSize; i++)
        {
            if (fscanf(fp, "%c", &characterSet[i]) != 1)
            {
                fprintf(stderr, "\nError caused by reading characterSet!"
                        "Press any key to return...");
                getch();
                return huffmanArray;
            }
            if (characterSet[i] == '\r')
                i--;
        }

        for (i = 0; i < characterSetSize; i++)
            if (fscanf(fp, "%u", &characterSetWeight[i]) != 1)
            {
                fprintf(stderr, "\nError caused by reading characterSetWeight!"
                        "Press any key to return...");
                getch();
                return huffmanArray;
            }
        fclose(fp);

        //initialize huffmanArray's primary character node
        for (p = huffmanArray + 1, i = 1; i <= characterSetSize; ++i, ++p)
        {
            p->weight = characterSetWeight[i - 1];
            p->value = characterSet[i - 1];
            p->parent = 0;
            p->lchild = 0;
            p->rchild = 0;
            p->size = 1;  //primary size is 1
        }
        //initialize huffmanArray's generated node
        for ( ; i <= huffmanArraySize; ++i, ++p)
        {
            p->weight = 0;    //empty
            p->value = ' ';   //space
            p->parent = 0;
            p->lchild = 0;
            p->rchild = 0;
            p->size = 1;
        }

        for (i = characterSetSize + 1; i <= huffmanArraySize; i++)
        {
            //select the min and secondMin node from huffmanArray
            select(huffmanArray, i - 1, &min1, &min2);
            //generate new tree and update some information
            huffmanArray[min1].parent = i;
            huffmanArray[min2].parent = i;
            huffmanArray[i].size = huffmanArray[min1].size + huffmanArray[min2].size + 1;
            huffmanArray[i].lchild = min1;
            huffmanArray[i].rchild = min2;
            huffmanArray[i].weight = huffmanArray[min1].weight + huffmanArray[min2].weight;
        }

        if ((fp = fopen(huffmanTreeFile, "wb")) == NULL)
        {
            fprintf(stderr, "\nError caused by opening huffmanTreeFile!"
                    "Press any key to return...\n");
            getch();
            return huffmanArray;
        }
        rewind(fp);
        //print huffmanArray's size for using of reading huffmanTreeFile
        fprintf(fp, "%d\r\n", huffmanArraySize);
        //print all node's information
        for (i = 1; i <= huffmanArraySize; i++)
            fprintf(fp, "%d\t%d\t%c\t%u\t%u\t%u\t%u\r\n", i, huffmanArray[i].weight, huffmanArray[i].value,
                    huffmanArray[i].parent, huffmanArray[i].lchild, huffmanArray[i].rchild, huffmanArray[i].size);
        fflush(fp);
        fclose(fp);
    }
    free(characterSet);
    free(characterSetWeight);
    return huffmanArray;
}

/*Name: select
 *Interface:
 *    Arguments:
 *        HuffmanTreeNode *huffmanArray
 *        int boundary             : select from array's item from 1 to boundary
 *        int *minWeightIndex      :
 *        int*secondMinWeightIndex
 *    No returns
 *Use: travel array's part to find array's items with min weight and secondMin weight
 *Else:
 */
void select(HuffmanTreeNode *huffmanArray, int boundary, int *minWeightIndex,
            int*secondMinWeightIndex)
{
    int i, j;   //counter

    //find first item whose parent is 0
    for (j = 1; j <= boundary && huffmanArray[j].parent != 0; j++)
        ;
    *minWeightIndex = i = j;
    //travel the part and update indexs
    for (i++; i <= boundary; i++)
        //parent must be 0
        if (huffmanArray[i].parent == 0)
        {
            //compare rules: weight > size
            if (huffmanArray[i].weight < huffmanArray[*minWeightIndex].weight)
            {
                *secondMinWeightIndex = *minWeightIndex;
                *minWeightIndex = i;
            }
            //if weight equals, then compare their sizes
            else if (huffmanArray[i].weight == huffmanArray[*minWeightIndex].weight)
            {
                if (huffmanArray[i].size < huffmanArray[*minWeightIndex].size)
                {
                    *secondMinWeightIndex = *minWeightIndex;
                    *minWeightIndex = i;
                }
                //update *secondMinWeightIndex
                else
                    *secondMinWeightIndex = i;
            }
            else
                ;
        }

    //if *minWeightIndex not changed, then *secondMinWeightIndex is empty,
    //so travel from (j+1) and find the index.
    if (j == *minWeightIndex)
    {
        //initialize its value with endIndex because endIndex's parent is 0 and endIndex isn't *minWeightIndex
        *secondMinWeightIndex = boundary;
        //skip the *minWeightIndex and travel
        for (j++; j < boundary; j++)
        {
            if (huffmanArray[j].parent == 0)
            {
                //same compare rules.
                if (huffmanArray[j].weight < huffmanArray[*secondMinWeightIndex].weight)
                    *secondMinWeightIndex = j;
                else if (huffmanArray[j].weight == huffmanArray[*secondMinWeightIndex].weight)
                {
                    if (huffmanArray[j].size < huffmanArray[*secondMinWeightIndex].size)
                        *secondMinWeightIndex = j;
                }
                else
                    ;
            }
        }
    }
}

/*Name: encode
 *Interface:
 *    Arguments:
 *        HuffmanTreeNode *huffmanArray :
 *        int size                      : characterSet's size
 *        char *fileRes                 : file to encode
 *        char *fileDes                 : file encoded
 *    Return : if encode mission completed successfully
 *Use: Based on huffmanArray, encode from fileRes to fileDes
 *     First, find the encoding character's index in huffmanArray,
 *     Second, travel from child to parent and save '0' or '1' to string
 *     by reverse order, Finally, print the string to fileDes.
 *Else: if error occurs when opening file, then return FALSE
 *      if the character is not in characterSet then exit(EXIT_FAILURE);
 */
Bool encode(HuffmanTreeNode *huffmanArray, int size, char *fileRes, char *fileDes)
{
    FILE *fpIn;   //file stream for input
    FILE *fpOut;  //file stream for output
    char c;       //character has read
    char temp[size];  //temp string to save current character's huffman code
    int start;    //current character's huffman code's start position in string
    int parent;
    int child;
    int index;   //current character's index in huffmanArray

    if (huffmanArray == NULL)
    {
        fprintf(stderr, "\nhuffmanArray is NULL!"
                "Press any key to return...\n");
        getch();
        return FALSE;
    }
    if ((fpIn = fopen(fileRes, "rb")) == NULL)
    {
        fprintf(stderr, "\nError caused by opening encodeFileRes!"
                "Press any key to return...\n");
        getch();
        return FALSE;
    }
    if ((fpOut = fopen(fileDes, "wb")) == NULL)
    {
        fprintf(stderr, "\nError caused by opening encodeFileDes!"
                "Press any key to return...\n");
        getch();
        return FALSE;
    }
    rewind(fpIn);
    rewind(fpOut);
    //when error occurs or file is over, loop exits
    while (!feof(fpIn) && !ferror(fpOut) && !ferror(fpOut))
    {
        int i;
        c = fgetc(fpIn);    //read character from file
        if (c == '\r')
            continue;
        //find current character's index
        for (i = 1; i <= size; i++)
            if (huffmanArray[i].value == c)
                break;
        //if the character is not in characterSet
        if (i > size && !feof(fpIn))
        {
            fprintf(stderr, "\nError occurs because of invalid character %c!"
                    "Perss Any key to exit...", c);
            getch();
            exit(EXIT_FAILURE);
        }
        index = i;
        temp[size - 1] = '\0';
        start = size - 1;
        for (child = index, parent = huffmanArray[index].parent;
                parent != 0;
                child = parent, parent = huffmanArray[parent].parent)
        {
            if (huffmanArray[parent].lchild == child)
                temp[--start] = '0';
            else
                temp[--start] = '1';
        }
        fwrite(&temp[start], sizeof(char), size - start - 1, fpOut);
//      or fprintf(fpOut, "%s", &temp[start]);
    }
    fclose(fpIn);
    fflush(fpOut);
    fclose(fpOut);
    return TRUE;
}

/*Name: decode
 *Interface:
 *    Arguments:
 *         HuffmanTreeNode *huffmanArray
 *         int size      : huffmanArray's size
 *         char *fileRes : file to decode
 *         char *fileDes : file decoded
 *    Return : if decode mission completed successfully
 *Use: Based on huffmanArray, decode fileRed to fileDes
 *     Each time read a character from fileRes, if equals '0' then goes to cursor's lchild,
 *     if equals '1' then goes to cursor's rchild until cursor goes to leafs.
 *Else: if error occurs when opening file, then return FALSE
 */
Bool decode(HuffmanTreeNode *huffmanArray, int size, char *fileRes, char *fileDes)
{
    FILE *fpIn;
    FILE *fpOut;
    char c;
    int cursor = size;

    if (huffmanArray == NULL)
    {
        fprintf(stderr, "\nhuffmanArray is NULL!"
                "Press any key to return...\n");
        getch();
        return FALSE;
    }

    if ((fpIn = fopen(fileRes, "rb")) == NULL)
    {
        fprintf(stderr, "\nError caused by opening encodeFileDes!"
                "Press any key to return...\n");
        getch();
        return FALSE;
    }
    if ((fpOut = fopen(fileDes, "wb")) == NULL)
    {
        fprintf(stderr, "\nError caused by opening decodeFileDes!"
                "Press any key to return...\n");
        getch();
        return FALSE;
    }
    rewind(fpIn);
    rewind(fpOut);
    while (!feof(fpIn) && !ferror(fpIn) && !ferror(fpOut))
    {
        c = fgetc(fpIn);
        //if cursor goes to leaf
        if (huffmanArray[cursor].lchild == 0 && huffmanArray[cursor].rchild == 0)
        {
            //print the value and move cursor to root
            fputc(huffmanArray[cursor].value, fpOut);
            cursor = size;

        }
        if (c == '0')
            cursor = huffmanArray[cursor].lchild;
        else if (c == '1')
            cursor = huffmanArray[cursor].rchild;
        else
            ;
    }

    fflush(fpOut);
    fclose(fpIn);
    fclose(fpOut);
    return TRUE;
}

/*Name: printCode
 *Interface:
 *    Arguments:
 *        char *fileRes
 *        char *fileDes
 *    Return : if the print mission completed successfully
 *Use: Open the resource file and print it to screen and destination file
 *     by formating.
 *Else:
 */
Bool printCode(char *fileRes, char *fileDes)
{
    FILE *fpIn;
    FILE *fpOut;
    char c;

    if ((fpIn = fopen(fileRes, "rb")) == NULL)
    {
        fprintf(stderr, "\nError caused by opening encodeFileDes!"
                "Press any key to return...\n");
        getch();
        return FALSE;
    }
    if ((fpOut = fopen(fileDes, "wb")) == NULL)
    {
        fprintf(stderr, "\nError caused by opening printFileDes!"
                "Press any key to return...\n");
        getch();
        return FALSE;
    }
    rewind(fpIn);
    rewind(fpOut);
    while (!feof(fpIn) && !ferror(fpIn) && !ferror(fpOut))
    {
        //format
        for (int i = 0; i < 50; i++)
        {
            c = fgetc(fpIn);
            printf("%c", c);
            fprintf(fpOut, "%c", c);
        }
        printf("\n");
        fprintf(fpOut, "\r\n");
    }

    fflush(fpOut);
    fclose(fpIn);
    fclose(fpOut);
    printf("\nPress any key to return...");
    getch();
}

/*Name: printHuffmanTree
 *Interface:
 *    Argument:
 *        HuffmanTreeNode *huffmanArray
 *        int size
 *        char *fileDes
 *    Return: if the print mission completed successfully.
 *Use: print the tree and save data to file.
 *Else:
 */
Bool printHuffmanTree(HuffmanTreeNode *huffmanArray, int size, char *fileDes)
{
    FILE *fp;

    if ((fp = fopen(fileDes, "wb")) == NULL)
    {
        fprintf(stderr, "\nError caused by opening printHuffmanTreeDes!"
                "Press any key to return...\n");
        getch();
        return FALSE;
    }
    printTree(huffmanArray, size, size, 0, fp);
    printf("\nPress any key to return...");
    getch();
    fflush(fp);
    fclose(fp);
}

/*Name: printTree
 *Interface:
 *    Arguments:
 *        HuffmanTreeNode *huffmanArray
 *        int i
 *        int size
 *        int depth
 *        FILE *fp
 *    No return
 *Use: travel and print the tree recursively and print data to file
 *Else:
 */
void printTree(HuffmanTreeNode *huffmanArray, int i, int size, int depth, FILE *fp)
{
    //use argument "depth" to format when printing.
    printf("\n");
    fprintf(fp, "\r\n");
    for (int j = 0; j < depth; j++)
    {
        printf("    ");
        fprintf(fp, "    ");
    }
    //if it goes to leaf then print the value
    //else print the weight
    if (i >= 1 && i <= ((size + 1) / 2))
    {
        printf("%c", huffmanArray[i].value);
        fprintf(fp, "%c", huffmanArray[i].value);
    }
    else
    {
        printf("%u", huffmanArray[i].weight);
        fprintf(fp, "%u", huffmanArray[i].weight);
    }
    //if goes to leaf then return
    if (huffmanArray[i].lchild == 0 && huffmanArray[i].rchild == 0)
        return;
    printTree(huffmanArray, huffmanArray[i].lchild, size, depth + 1, fp);
    printTree(huffmanArray, huffmanArray[i].rchild, size, depth + 1, fp);
}
