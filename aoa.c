#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>

#define MAX_STUDENTS 100
#define NAME_LEN     30
#define LINE_LEN     256

enum Field { BY_ID = 1, BY_NAME, BY_MARKS };
enum Order { ASCENDING = 1, DESCENDING };
enum Pivot { PIVOT_FIRST = 1, PIVOT_MIDDLE, PIVOT_LAST };

typedef struct {
    int   id;
    char  name[NAME_LEN];
    float marks;
} Student;

typedef struct {
    long comparisons;
    long swaps;
    long recursiveCalls;
    int  maxDepth;
} Stats;

static Student students[MAX_STUDENTS];
static int     count = 0;

/* user settings */
static int sortField   = BY_ID;
static int sortOrder   = ASCENDING;
static int pivotChoice = PIVOT_FIRST;

static Stats stats;


/* ------------------------------------------------------------------ */
/*  Safe input helpers (no raw scanf, so bad input can't hang the app) */
/* ------------------------------------------------------------------ */

/* Read one line; discards any overflow; exits cleanly on EOF. */
static void readLine(const char *prompt, char *buf, size_t size)
{
    printf("%s", prompt);
    fflush(stdout);

    if (!fgets(buf, (int)size, stdin))
    {
        printf("\nInput closed. Exiting.\n");
        exit(0);
    }

    if (strchr(buf, '\n'))
        buf[strcspn(buf, "\n")] = '\0';
    else
    {
        int ch;                                   /* line was too long: drain it */
        while ((ch = getchar()) != '\n' && ch != EOF)
            ;
    }
}

static int onlySpaceLeft(const char *p)
{
    while (*p && isspace((unsigned char)*p)) p++;
    return *p == '\0';
}

/* Keep asking until the user enters an integer in [min, max]. */
static int readInt(const char *prompt, int min, int max)
{
    char line[LINE_LEN], *end;

    for (;;)
    {
        readLine(prompt, line, sizeof line);

        errno = 0;
        long v = strtol(line, &end, 10);

        if (end != line && onlySpaceLeft(end) && errno == 0 && v >= min && v <= max)
            return (int)v;

        printf("Invalid input. Enter a whole number from %d to %d.\n", min, max);
    }
}

/* Keep asking until the user enters a number in [min, max]. */
static float readFloat(const char *prompt, float min, float max)
{
    char line[LINE_LEN], *end;

    for (;;)
    {
        readLine(prompt, line, sizeof line);

        errno = 0;
        float v = strtof(line, &end);

        if (end != line && onlySpaceLeft(end) && errno == 0 && v >= min && v <= max)
            return v;

        printf("Invalid input. Enter a number from %.0f to %.0f.\n", min, max);
    }
}

/* Read a non-empty name (longer names are truncated to fit). */
static void readName(const char *prompt, char *dest)
{
    char line[LINE_LEN];

    for (;;)
    {
        readLine(prompt, line, sizeof line);

        char *p = line;
        while (isspace((unsigned char)*p)) p++;          /* trim leading spaces  */

        size_t len = strlen(p);
        while (len > 0 && isspace((unsigned char)p[len - 1]))
            p[--len] = '\0';                              /* trim trailing spaces */

        if (len > 0)
        {
            if (len > NAME_LEN - 1) len = NAME_LEN - 1;   /* truncate long names */
            memcpy(dest, p, len);
            dest[len] = '\0';
            return;
        }
        printf("Name cannot be empty.\n");
    }
}

static int idExists(int id)
{
    for (int i = 0; i < count; i++)
        if (students[i].id == id)
            return 1;
    return 0;
}


/* ------------------------------------------------------------------ */
/*  Quick Sort                                                         */
/* ------------------------------------------------------------------ */

/* Compare two students: returns <0, 0 or >0 (no overflow possible) */
static int compare(const Student *a, const Student *b)
{
    int result;
    stats.comparisons++;

    if (sortField == BY_ID)
        result = (a->id > b->id) - (a->id < b->id);
    else if (sortField == BY_NAME)
    {
        int c = strcmp(a->name, b->name);
        result = (c > 0) - (c < 0);
    }
    else
        result = (a->marks > b->marks) - (a->marks < b->marks);

    return (sortOrder == DESCENDING) ? -result : result;
}

static void swap(Student *a, Student *b)
{
    Student temp = *a;
    *a = *b;
    *b = temp;
    stats.swaps++;
}

/* Pick the pivot position based on the user's choice */
static int choosePivot(int low, int high)
{
    if (pivotChoice == PIVOT_FIRST)  return low;
    if (pivotChoice == PIVOT_MIDDLE) return low + (high - low) / 2;
    return high;
}

/* Lomuto partition: items <= pivot go left, the rest go right */
static int partition(int low, int high)
{
    int pivotIndex = choosePivot(low, high);
    Student pivot  = students[pivotIndex];

    if (pivotIndex != high)                       /* avoid a pointless self-swap */
        swap(&students[pivotIndex], &students[high]);

    int i = low - 1;                              /* end of the "smaller" section */

    for (int j = low; j < high; j++)
    {
        if (compare(&students[j], &pivot) <= 0)
        {
            i++;
            if (i != j)
                swap(&students[i], &students[j]);
        }
    }

    if (i + 1 != high)
        swap(&students[i + 1], &students[high]);  /* pivot to its final place */

    return i + 1;
}

static void quickSort(int low, int high, int depth)
{
    if (low >= high)
        return;

    stats.recursiveCalls++;
    if (depth > stats.maxDepth)
        stats.maxDepth = depth;

    int p = partition(low, high);

    quickSort(low, p - 1, depth + 1);
    quickSort(p + 1, high, depth + 1);
}


/* ------------------------------------------------------------------ */
/*  Menu actions                                                       */
/* ------------------------------------------------------------------ */

static void display(void)
{
    if (count == 0)
    {
        printf("\nNo records available.\n");
        return;
    }

    printf("\n%-8s %-*s %-8s\n", "ID", NAME_LEN - 1, "NAME", "MARKS");
    for (int i = 0; i < count; i++)
        printf("%-8d %-*s %-8.2f\n", students[i].id, NAME_LEN - 1,
               students[i].name, students[i].marks);
}

static void addRecords(void)
{
    int remaining = MAX_STUDENTS - count;

    if (remaining == 0)
    {
        printf("\nStorage full (maximum %d records).\n", MAX_STUDENTS);
        return;
    }

    char prompt[64];
    snprintf(prompt, sizeof prompt, "\nHow many students (1-%d)? ", remaining);
    int toAdd = readInt(prompt, 1, remaining);

    for (int i = 0; i < toAdd; i++)
    {
        Student *st = &students[count];
        printf("\nStudent %d\n", count + 1);

        for (;;)
        {
            st->id = readInt("ID: ", 0, 2147483647);
            if (!idExists(st->id)) break;
            printf("ID %d already exists. Enter a different ID.\n", st->id);
        }

        readName("Name: ", st->name);
        st->marks = readFloat("Marks (0-100): ", 0.0f, 100.0f);

        count++;
    }
}

static void sortRecords(void)
{
    if (count == 0)
    {
        printf("\nNo records available.\n");
        return;
    }

    memset(&stats, 0, sizeof stats);              /* reset counters */

    quickSort(0, count - 1, 1);

    printf("\nRecords sorted successfully.\n");
    display();

    printf("\n--- Quick Sort Analysis ---\n");
    printf("Comparisons       : %ld\n", stats.comparisons);
    printf("Swaps             : %ld\n", stats.swaps);
    printf("Recursive Calls   : %ld\n", stats.recursiveCalls);
    printf("Maximum Depth     : %d\n",  stats.maxDepth);
}

static void selectField(void)
{
    printf("\n1. ID\n2. Name\n3. Marks\n");
    sortField = readInt("Choose field: ", 1, 3);
    if (count > 0) sortRecords();   /* sort immediately with the new setting */
}

static void selectOrder(void)
{
    printf("\n1. Ascending\n2. Descending\n");
    sortOrder = readInt("Choose order: ", 1, 2);
    if (count > 0) sortRecords();   /* sort immediately with the new setting */
}

static void selectPivot(void)
{
    printf("\n1. First Element\n2. Middle Element\n3. Last Element\n");
    pivotChoice = readInt("Choose pivot: ", 1, 3);
    if (count > 0) sortRecords();   /* sort immediately with the new setting */
}

static void showSettings(void)
{
    static const char *fields[] = { "", "ID", "Name", "Marks" };
    static const char *orders[] = { "", "Ascending", "Descending" };
    static const char *pivots[] = { "", "First", "Middle", "Last" };

    printf("Current: field=%s, order=%s, pivot=%s, records=%d/%d\n",
           fields[sortField], orders[sortOrder], pivots[pivotChoice],
           count, MAX_STUDENTS);
}


int main(void)
{
    int choice;

    do
    {
        printf("\n\n===== QUICK SORT RECORD ORGANIZER =====\n");
        showSettings();
        printf("1. Add Records\n");
        printf("2. Display Records\n");
        printf("3. Select Sorting Field\n");
        printf("4. Select Sorting Order\n");
        printf("5. Select Pivot\n");
        printf("6. Sort Records\n");
        printf("7. Exit\n");

        choice = readInt("Enter choice: ", 1, 7);

        switch (choice)
        {
            case 1: addRecords();   break;
            case 2: display();      break;
            case 3: selectField();  break;
            case 4: selectOrder();  break;
            case 5: selectPivot();  break;
            case 6: sortRecords();  break;
            case 7: printf("\nProgram ended.\n"); break;
        }

    } while (choice != 7);

    return 0;
}
