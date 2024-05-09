#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_RECORDS 100
#define STRING_LENGTH 50
#define FILENAME "UCHIHA_DATA.txt"

typedef struct {
    char name[STRING_LENGTH];
    int age_Of_Death;
    char gender;
    int sharingan;
    float rating;
} Uchiha;

// Function prototypes
void readDataFromFile(Uchiha **database, int *numRecords);
void displayMenu();
void searchRecords(Uchiha *database, int numRecords);
void sort_records(Uchiha *database, int numRecords);
void displayRecords(Uchiha *database, int numRecords);
void addRecord(Uchiha **database, int *numRecords);
void deleteRecord(Uchiha **database, int *numRecords);
void saveDataToFile(Uchiha *database, int numRecords);

int main(void) {
    Uchiha *database = NULL;
    int numRecords = 0;

    readDataFromFile(&database, &numRecords);

    char choice;
    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf(" %c", &choice);

        switch(choice) {
            case '1':
                searchRecords(database, numRecords);
                break;
            case '2':
                sort_records(database, numRecords);
                displayRecords(database, numRecords);
                break;
            case '3':
                displayRecords(database, numRecords);
                break;
            case '4':
                addRecord(&database, &numRecords);
                break;
            case '5':
                deleteRecord(&database, &numRecords);
                break;
            case '6':
                saveDataToFile(database, numRecords);
                break;
            case '0':
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while(choice != '0');

    // Free dynamically allocated memory
    free(database);

    return 0;
}

void readDataFromFile(Uchiha **database, int *numRecords) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("Error opening file %s.\n", FILENAME);
        return;
    }

    // Allocate memory for the database
    *database = (Uchiha *)malloc(MAX_RECORDS * sizeof(Uchiha));
    if (*database == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);

        return;
    }

    // Read data from file
    while ((*numRecords) < MAX_RECORDS &&
           fscanf(file, " %49[^,],%c,%d,%d,%f%*c",
                  (*database)[*numRecords].name,
                  &(*database)[*numRecords].gender,
                  &(*database)[*numRecords].age_Of_Death,
                  &(*database)[*numRecords].sharingan,
                  &(*database)[*numRecords].rating) !=EOF) {
        (*numRecords)++;

    }

    fclose(file);
}

void displayMenu() {
    printf("\nMenu:\n");
    printf("1. Search records\n");
    printf("2. Sort records\n");
    printf("3. Display records\n");
    printf("4. Add a record\n");
    printf("5. Delete a record\n");
    printf("6. Save data to file\n");
    printf("0. Quit\n");
}

void searchRecords(Uchiha *database, int numRecords) {
    char searchQuery[STRING_LENGTH];
    printf("Enter search query: ");
    scanf(" %s", searchQuery);

    printf("Matching record:\n");
    printf("%-20s %10s %10s %10s %6s \n", "Name","Age at Death","Gender","Sharingan","Rating");
    for (int i = 0; i < numRecords; i++) {
        if (strstr(database[i].name, searchQuery) != NULL ||
            database[i].gender == searchQuery[0] ||
            database[i].age_Of_Death == atoi(searchQuery) ||
            database[i].sharingan == atoi(searchQuery) ||
            database[i].rating == atof(searchQuery)) {
               printf("%-20s %7d %12c %10d %7.2f\n",
                database[i].name, database[i].age_Of_Death, database[i].gender,
                database[i].sharingan, database[i].rating);
               return; // Stop after printing the first matching record
        }
    }
    printf("No matching record found.\n");
}

void sort_records(Uchiha *database, int numRecords) {
    // Bubble sort
    for (int i = 0; i < numRecords - 1; i++) {
        for (int j = 0; j < numRecords - i - 1; j++) {
            if (strcmp(database[j].name, database[j + 1].name) > 0) {
                // Swap
                Uchiha temp = database[j];
                database[j] = database[j + 1];
                database[j + 1] = temp;
            }
        }
    }
    printf("Records sorted by name.\n");
}

void displayRecords(Uchiha *database, int numRecords) {
    printf("Displaying all records:\n");
    printf("%-20s %10s %10s %10s %6s \n", "Name","Age at Death","Gender","Sharingan","Rating");
    for (int i = 0; i < numRecords; i++) {
  printf("%-20s %7d %12c %10d %7.2f\n",
               database[i].name, database[i].age_Of_Death, database[i].gender,
               database[i].sharingan, database[i].rating);
    }
}

void addRecord(Uchiha **database, int *numRecords) {
    if (*numRecords >= MAX_RECORDS) {
        printf("Cannot add more records. Database is full.\n");
        return;
    }

    // Allocate memory for the new database with one additional record
    Uchiha *newDatabase = (Uchiha *)malloc(((*numRecords) + 1) * sizeof(Uchiha));
    if (newDatabase == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    // Copy existing records to the new database
    for (int i = 0; i < *numRecords; i++) {
        newDatabase[i] = (*database)[i];
    }

    // Free the memory of the old database
    free(*database);

    // Assign the pointer to the new database
    *database = newDatabase;

    // Increment the number of records
    (*numRecords)++;

    printf("Enter Uchiha clan member:\n");
    printf("Name: ");
    scanf(" %49[^\n]%*c", (*database)[*numRecords].name);
    printf("Age at Death: ");
    scanf("%d", &(*database)[*numRecords].age_Of_Death);
    printf("Gender: ");
    scanf(" %c", &(*database)[*numRecords].gender);
    printf("Sharingan: ");
    scanf("%d", &(*database)[*numRecords].sharingan);
    printf("Rating: ");
    scanf("%f", &(*database)[*numRecords].rating);

    (*numRecords)++;
    printf("Record added successfully.\n");
}

void deleteRecord(Uchiha **database, int *numRecords) {
    char deleteName[STRING_LENGTH];
    printf("Enter the name of the Uchiha clan member to delete: ");
    scanf(" %49[^\n]%*c", deleteName);

    int found = 0;
    for (int i = 0; i < *numRecords; i++) {
        if (strcmp((*database)[i].name, deleteName) == 0) {
            for (int j = i; j < *numRecords - 1; j++) {
                strcpy((*database)[j].name, (*database)[j + 1].name);
                (*database)[j].age_Of_Death = (*database)[j + 1].age_Of_Death;
                (*database)[j].gender = (*database)[j + 1].gender;
                (*database)[j].sharingan = (*database)[j + 1].sharingan;
                (*database)[j].rating = (*database)[j + 1].rating;
            }
            (*numRecords)--;
            found = 1;
            printf("Record deleted successfully.\n");
            return;
        }
    }

    if (!found) {
        printf("Record not found.\n");
    }
}

void saveDataToFile(Uchiha *database, int numRecords) {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error opening file %s for writing.\n", FILENAME);
        return;
    }

    for (int i = 0; i < numRecords; i++) {
        fprintf(file, "%s,%d,%c,%d,%.2f\n",
                database[i].name, database[i].age_Of_Death, database[i].gender,
                database[i].sharingan, database[i].rating);
    }

    fclose(file);
    printf("Data saved to file %s successfully.\n", FILENAME);
}