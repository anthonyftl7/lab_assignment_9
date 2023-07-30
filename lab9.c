#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct RecordType 
{
    int id;
    char name;
    int order;
};

struct HashType 
{
    struct RecordType **data;
};

int hash(int x, int hashSz) 
{
    return rand() * x % hashSz;
}

// parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData) 
{
    FILE *inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile) {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType *) malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL) {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i) {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz) 
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i) 
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz) 
{
    int i;

    for (i = 0; i < hashSz; ++i) {
        if (pHashArray->data[i] == NULL) 
            continue;

        printf("index %d-> ", i);
        for (int j = 0; pHashArray->data[i][j].id != 0; ++j) 
            printf(" [%d, %c, %d] ", pHashArray->data[i][j].id,pHashArray->data[i][j].name, pHashArray->data[i][j].order);
        printf("\n");
    }
}

int main(void)
{

    srand(time(NULL));

    struct RecordType* pRecords;
    int recordSz = 0;
    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    struct HashType hashTable;
    hashTable.data = (void*) calloc(recordSz, sizeof(struct RecordType*));

    for (unsigned int i = 0; i < recordSz; ++i) 
        hashTable.data[i] = NULL;

    for (int i = 0; i < recordSz; ++i) {
        int h = hash(pRecords[i].id, recordSz);
        int j = 0;

        while (hashTable.data[h] != NULL && hashTable.data[h][j].id != 0) 
            ++j;

        if (hashTable.data[h] == NULL) {
            hashTable.data[h] = (void*) calloc(recordSz + 1, sizeof(struct RecordType));
            hashTable.data[h][0].id = 0;
        }

        hashTable.data[h][j] = pRecords[i];
        hashTable.data[h][j + 1].id = 0;
    }

    displayRecordsInHash(&hashTable, recordSz);

    for (int i = 0; i < recordSz; ++i) 
        free(hashTable.data[i]);

    free(hashTable.data);
    free(pRecords);

    return 0;
}
