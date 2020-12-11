#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int cols;
int rows;
char *AND = "AND";
char *NOT = "NOT";
char *OR = "OR";
char *NAND = "NAND";
char *NOR = "NOR";
char *XOR = "XOR";

typedef struct inputNode
{
    char inputName[11];
    int colNumber;
    struct inputNode *next;
} inputNode;


typedef struct outputNode
{
    char outputName[11];
    int *result;
    struct outputNode *next;
} outputNode;

inputNode *headInput = NULL;
outputNode *hTempVars = NULL;
outputNode *headOutput = NULL;

void puSH(outputNode **head, char *name, int rows)
{
    outputNode *output = (outputNode *)malloc(sizeof(outputNode));
    memset(output, 0, sizeof(outputNode));
    int *subMatrix = calloc(rows, sizeof(int));
    output->result = subMatrix;
    strcpy(output->outputName, name);
    for (int i = 0; i < rows; i++)
        subMatrix[i] = 0;

    if (*head == NULL)
    {
        *head = output;
        return;
    }

    outputNode *temp = *head, *prev = *head;
    while (temp != NULL)
    {
        prev = temp;
        temp = temp->next;
    }
    if (prev != NULL)
        prev->next = output;
}

void puSH2(outputNode **head, char name[11], int rows)
{
    outputNode *output = (outputNode *)malloc(sizeof(outputNode));
    memset(output, 0, sizeof(outputNode));
    int *subMatrix = calloc(rows, sizeof(int));
    output->result = subMatrix;
    strcpy(output->outputName, name);
    for (int i = 0; i < rows; i++)
        subMatrix[i] = 0;
    output->next = NULL;
    if (*head == NULL)
    {
        *head = output;
        return;
    }

    outputNode *temp = *head, *prev = *head;
    while (temp != NULL)
    {
        prev = temp;
        temp = temp->next;
    }
    if (prev != NULL)
        prev->next = output;
}

void push(inputNode **head, char *name, int col)
{
    inputNode *newNode = (inputNode *)malloc(sizeof(inputNode));
    memset(newNode, 0, sizeof(inputNode));
    if (newNode == NULL)
        return;
    newNode->colNumber = col;
    strcpy(newNode->inputName, name);
    if (*head == NULL)
    {
        *head = newNode;
        return;
    }

    inputNode *temp = *head, *prev = *head;
    while (temp != NULL)
    {
        prev = temp;
        temp = temp->next;
    }
    if (prev != NULL)
        prev->next = newNode;
   
}

void traverseList(outputNode *head)
{
    outputNode *temp;

    // Return if list is empty
    if (head == NULL)
    {
        printf("List is empty.");
        return;
    }

    temp = head;
    while (temp != NULL)
    {
        printf("Data = %s\n", temp->outputName); 
        temp = temp->next;                  
    }
}

void freeInput(inputNode *head)
{
    inputNode *temp, *next;
    if (head == NULL)
        return;
    temp = head;
    while (temp != NULL)
    {
        next = temp;
        temp = temp->next;
        free(next);
    }
}


void freeOutPut(outputNode **head)
{
    outputNode *temp, *next;
    temp = next = NULL;

    // Return if list is empty
    if (head == NULL || *head == NULL)
    {
        return;
    }
    temp = *head;
    while (temp != NULL)
    {
        next = temp;
        temp = temp->next;
        if (next->result != NULL)
            free(next->result);
        if (next != NULL)
            free(next);
    }
}
int name(inputNode *head, char name[10])
{
    inputNode *temp;
    temp = head;
    while (temp != NULL)
    {
        if (strcmp(temp->inputName, name) == 0)
            return temp->colNumber;
        temp = temp->next;
    }
    return -1;
}
outputNode *name2(outputNode **head, char *name)
{
    outputNode *temp = NULL;
    if (name == NULL)
        return NULL;
    if (head == NULL)
        return NULL;
    temp = *head;
    while (temp != NULL)
    {

        if (strcmp(temp->outputName, name) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}


void printMatrix(outputNode *head)
{
    outputNode *temp;
    if (head == NULL)
    {
        printf("error");
        return; //exit(-1);
    }
    temp = head;
    if (temp->result == NULL)
    {
        printf("empty data");
        exit(-1);
    }

    while (temp != NULL)
    {
        printf("%s ", temp->outputName);
        //  m = temp->result;
        for (int i = 0; i < rows; i++)
        {

            printf("%d ", temp->result[i]);
        }
        printf("\n");
        temp = temp->next;
    }
    printf("OUTPUT\n");
    for (int z = 0; z < rows; ++z)
    {
        temp = head;

        while (temp != NULL)
        {
            printf("%d\t", temp->result[z]);
            temp = temp->next;
        }
        printf("\n");
    }
}
void replaceMatrix(outputNode *head, char *name, int replace[rows])
{
    outputNode *temp;
    temp = head;
    while (temp != NULL)
    {
        if (strcmp(temp->outputName, name) == 0)
        {

            for (int i = 0; i < rows; i++)
            {
                temp->result[i] = replace[i];
            }
            return;
        }

        temp = temp->next;
    }
}
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
void fillMatrix(int **matrix, int rows, int cols)
{

    for (int i = (1 << cols) - 1; i >= 0; i--)
    {
        for (int level = 0; level < cols; level++)
        {

            matrix[i][level] = (i >> level) & 1;
        }
    }

    for (int r = 0; r < rows; r++)
    {
        int start = 0;
        int end = cols - 1;
        while (start < end)
        {
            swap(&matrix[r][start], &matrix[r][end]);
            start++;
            end--;
        }
    }
}

int main(int argc, char *argv[])
{

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("invalid file");
        exit(-1);
    }

    int numOutPuts = 0;

    char singleLine[500];
    char *token;

    int tempLen = 0;

    int **matrix;

    while (fgets(singleLine, 500, fp) != NULL)
    {
        token = strtok(singleLine, " \n");
        while (token != NULL)
        {
            if (strcmp(token, "INPUTVAR") == 0)
            {
                token = strtok(NULL, " \n");
                cols = atoi(token);
                rows = 1 << cols;

                matrix = calloc(rows, sizeof(int *));
                for (int i = 0; i < rows; i++)
                    matrix[i] = calloc(cols, sizeof(int));

                fillMatrix(matrix, rows, cols);

                token = strtok(NULL, " \n");
                for (int c = 0; c < cols; c++)
                {
                    char *temp = token;        
                    push(&headInput, temp, c); 
                    token = strtok(NULL, " \n");
                }
            }
            else if (strcmp(token, "OUTPUTVAR") == 0)
            {
                token = strtok(NULL, " \n");
                numOutPuts = atoi(token);
                token = strtok(NULL, " \n");

                for (int i = 0; i < numOutPuts; i++)
                {
                    char *temp = token;
                    puSH(&headOutput, temp, rows);
                    token = strtok(NULL, " \n");
                }
               
            }
            else 
            {
                char *opName = token;
                token = strtok(NULL, " \n");

                char *input1 = token;
                token = strtok(NULL, " \n");

                char *input2;
                if (strcmp(opName, NOT) != 0)
                {
                    input2 = token;
                    token = strtok(NULL, " \n");
                }
                char *tempVar = token;
                if (tempVar == NULL)
                    exit(-1);
                outputNode *exists;
                if (headOutput == NULL)
                {
                    exit(-1);
                }
                else
                {
                    exists = name2(&headOutput, tempVar); //if it is X or Y
                }
                int IsItOutlist = 0;
                if (exists == NULL)
                {
                    puSH(&hTempVars, tempVar, rows);
                }
                else
                {
                    IsItOutlist = 1;
                }
                int col1;
                outputNode *ptr1 = NULL;
                outputNode *ptr2 = NULL;
                int col2;
                col1 = name(headInput, input1);
                if (col1 == -1)
                    ptr1 = name2(&hTempVars, input1);
                if (strcmp(opName, NOT) != 0)
                {
                    col2 = name(headInput, input2);
                    if (col2 == -1)
                        ptr2 = name2(&hTempVars, input2);
                }
                int subMatrix[rows];

                int rows2 = 0;
                int res1 = 0;
                int res2 = 0;
                int finres;
                for (int i = 0; i < rows; i++)
                {
                    res1 = res2 = 0;
                    if (col1 != -1)
                        res1 = matrix[rows2][col1];
                    else if (ptr1 != NULL)
                        res1 = ptr1->result[i];
                    if (strcmp(opName, NOT) != 0)
                    {
                        if (col2 != -1)
                            res2 = matrix[rows2][col2];
                        else if (ptr2 != NULL)
                            res2 = ptr2->result[i];
                    }
                    if (strcmp(opName, AND) == 0)
                        finres = res1 && res2;
                    else if (strcmp(opName, OR) == 0)
                        finres = res1 || res2;
                    else if (strcmp(opName, NAND) == 0)
                    {
                        finres = res1 && res2;
                        finres = !finres;
                    }
                    else if (strcmp(opName, NOR) == 0)
                        finres = !(res1 || res2);
                    else if (strcmp(opName, XOR) == 0)
                        finres = res1 ^ res2;
                    else if (strcmp(opName, NOT) == 0)
                        finres = !res1;
                    subMatrix[i] = finres;
                    rows2++;
                }
                if (IsItOutlist == 0)
                    replaceMatrix(hTempVars, tempVar, subMatrix);
                else
                    replaceMatrix(headOutput, tempVar, subMatrix);
            }

            token = strtok(NULL, " \n");
        }
    }

    int secCols = cols + numOutPuts;
    int **finalResult = calloc(rows, sizeof(int *));
    for (int i = 0; i < rows; i++)
        finalResult[i] = calloc(secCols, sizeof(int));

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            finalResult[r][c] = matrix[r][c];
        }
    }

    
    outputNode *traverse = headOutput;
    int outcol = cols;
    while (traverse != NULL)
    {
        if (outcol >= secCols)
            break;
        for (int i = 0; i < rows; i++)
        {
            finalResult[i][outcol] = traverse->result[i];
        }
        traverse = traverse->next;
        outcol++;
    }
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < secCols; c++)
        {
            printf("%d ", finalResult[r][c]);
        }
        printf("\n");
    }

    for (int i = 0; i < rows; i++)
        free(matrix[i]);
    free(matrix);

    for (int i = 0; i < rows; i++)
        free(finalResult[i]);
    free(finalResult);

    freeInput(headInput);
    freeOutPut(&headOutput);
    freeOutPut(&hTempVars);
    fclose(fp); 
}
