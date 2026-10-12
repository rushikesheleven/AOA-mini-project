#include <stdio.h>
#include <string.h>

#define MAX 100

struct Student {
    int id;
    char name[30];
    float marks;
};

struct Student s[MAX];
int n = 0;                              // number of records

int field = 1, order = 1, pivotChoice = 1;   // user settings

// Counters for analysis
long comparisons = 0, swaps = 0, recursiveCalls = 0;
int maxDepth = 0;


/* Compare two students: returns <0, 0 or >0 */
int compare(struct Student *a, struct Student *b)
{
    int result;
    comparisons++;

    if (field == 1)
        result = a->id - b->id;
    else if (field == 2)
        result = strcmp(a->name, b->name);
    else
        result = (a->marks > b->marks) - (a->marks < b->marks);

    return (order == 2) ? -result : result;   // flip for descending
}


/* Swap two students */
void swap(struct Student *a, struct Student *b)
{
    struct Student temp = *a;
    *a = *b;
    *b = temp;
    swaps++;
}


/* Pick pivot position based on user's choice */
int choosePivot(int low, int high)
{
    if (pivotChoice == 1) return low;
    if (pivotChoice == 2) return (low + high) / 2;
    return high;
}


/* Partition: put smaller items left of pivot, larger to the right */
int partition(int low, int high)
{
    int pivotIndex = choosePivot(low, high);
    struct Student pivot = s[pivotIndex];

    swap(&s[pivotIndex], &s[high]);        // move pivot to the end

    int i = low - 1;                       // end of the "smaller" section

    for (int j = low; j < high; j++)
    {
        if (compare(&s[j], &pivot) <= 0)
        {
            i++;
            if (i != j)
                swap(&s[i], &s[j]);
        }
    }

    swap(&s[i + 1], &s[high]);             // put pivot in its final place
    return i + 1;
}


/* Quick Sort */
void quickSort(int low, int high, int depth)
{
    if (low >= high)
        return;

    recursiveCalls++;
    if (depth > maxDepth)
        maxDepth = depth;

    int p = partition(low, high);

    quickSort(low, p - 1, depth + 1);      // sort left part
    quickSort(p + 1, high, depth + 1);     // sort right part
}


/* Show all records */
void display()
{
    if (n == 0)
    {
        printf("\nNo records available.\n");
        return;
    }

    printf("\n%-8s %-20s %-8s\n", "ID", "NAME", "MARKS");
    for (int i = 0; i < n; i++)
        printf("%-8d %-20s %-8.2f\n", s[i].id, s[i].name, s[i].marks);
}


/* Add new records */
void addRecords()
{
    int count;

    printf("\nHow many students? ");
    scanf("%d", &count);

    if (n + count > MAX)
    {
        printf("Maximum %d records allowed.\n", MAX);
        return;
    }

    for (int i = 0; i < count; i++)
    {
        printf("\nStudent %d\n", n + 1);

        printf("ID: ");
        scanf("%d", &s[n].id);

        printf("Name: ");
        scanf(" %[^\n]", s[n].name);

        printf("Marks: ");
        scanf("%f", &s[n].marks);

        n++;
    }
}


/* Sort and show analysis */
void sortRecords()
{
    if (n == 0)
    {
        printf("\nNo records available.\n");
        return;
    }

    comparisons = swaps = recursiveCalls = maxDepth = 0;   // reset counters

    quickSort(0, n - 1, 1);

    printf("\nRecords sorted successfully.\n");
    display();

    printf("\n--- Quick Sort Analysis ---\n");
    printf("Comparisons       : %ld\n", comparisons);
    printf("Swaps             : %ld\n", swaps);
    printf("Recursive Calls   : %ld\n", recursiveCalls);
    printf("Maximum Depth     : %d\n", maxDepth);
}


/* Read a choice between 1 and max; invalid input becomes 1 */
int readOption(int max)
{
    int x;
    printf("Choose %s: ", "option");
    scanf("%d", &x);
    return (x < 1 || x > max) ? 1 : x;
}


void selectField()
{
    printf("\n1. ID\n2. Name\n3. Marks\n");
    printf("Choose field: ");
    scanf("%d", &field);
    if (field < 1 || field > 3) field = 1;
}

void selectOrder()
{
    printf("\n1. Ascending\n2. Descending\n");
    printf("Choose order: ");
    scanf("%d", &order);
    if (order < 1 || order > 2) order = 1;
}

void selectPivot()
{
    printf("\n1. First Element\n2. Middle Element\n3. Last Element\n");
    printf("Choose pivot: ");
    scanf("%d", &pivotChoice);
    if (pivotChoice < 1 || pivotChoice > 3) pivotChoice = 1;
}


int main()
{
    int choice;

    do
    {
        printf("\n\n===== QUICK SORT RECORD ORGANIZER =====\n");
        printf("1. Add Records\n");
        printf("2. Display Records\n");
        printf("3. Select Sorting Field\n");
        printf("4. Select Sorting Order\n");
        printf("5. Select Pivot\n");
        printf("6. Sort Records\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1: addRecords();   break;
            case 2: display();      break;
            case 3: selectField();  break;
            case 4: selectOrder();  break;
            case 5: selectPivot();  break;
            case 6: sortRecords();  break;
            case 7: printf("\nProgram ended.\n"); break;
            default: printf("\nInvalid choice.\n");
        }

    } while (choice != 7);

    return 0;
}