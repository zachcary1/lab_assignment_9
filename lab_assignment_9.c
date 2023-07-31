#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 31

#define NAME_MAX_LENGTH 20 // Define an appropriate maximum length for the name field

// RecordType
struct RecordType
{
    int id;
    char name[NAME_MAX_LENGTH]; // Define name as an array of characters
    int order;
    struct RecordType* next;
};

// Fill out this structure
struct HashType
{
    struct RecordType* head;
};

// Compute the hash function
int hash(int x)
{
    return x % (SIZE/2);
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char name[NAME_MAX_LENGTH]; // Temporary variable to store the name
    struct RecordType* pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%s ", name); // Read the name directly into the array
            strncpy(pRecord->name, name, NAME_MAX_LENGTH); // Copy the name to the record
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
        printf("\t%d %s %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType* pHashArray, int hashSz)
{
    int i;

    for (i = 0; i < hashSz; ++i)
    {
        struct RecordType* current = pHashArray[i].head;
        if (current != NULL)
        {
            printf("index %d -> ", i);
            while (current != NULL)
            {
                printf("%d %s %d -> ", current->id, current->name, current->order);
                current = current->next;
            }
            printf("\n");
        }
    }
}

int main(void)
{
    struct RecordType* pRecords;
    int recordSz = 0;
    int i;

    recordSz = parseData("input.txt", &pRecords);

    // Create the hash array
    struct HashType hashArray[SIZE];
    for (i = 0; i < SIZE; ++i)
    {
        hashArray[i].head = NULL;
    }

    // Insert records into the hash array using chaining
    for (i = 0; i < recordSz; ++i)
    {
        struct RecordType* pRecord = pRecords + i;
        int index = hash(pRecord->id);

        // Insert at the beginning of the linked list for chaining
        pRecord->next = hashArray[index].head;
        hashArray[index].head = pRecord;
    }

    printRecords(pRecords, recordSz); // Print original records
    displayRecordsInHash(hashArray, SIZE); // Display records in the hash structure

    free(pRecords);

    return 0;
}
