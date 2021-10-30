#include <ctype.h>
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h> //strlen
#include <malloc.h>
#include <unistd.h> //write
#include "config.h"

typedef struct
{
    char key[MAX_KEY_SIZE];
    char value[MAX_VALUE_SIZE];
} st_date;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int day;
    int month;
    int year;
} dateStruct;

typedef struct
{
    char title[100];
    char author[100];
    char publisher[100];
    char ISBN[30];
    dateStruct date;
    int copies;
    int current;
    char category[30];
} book;

int library_books(book book_array[])
{
    strcpy(book_array[0].title, "Automation and Robotics");
    strcpy(book_array[0].author, "Miltiadis Boboulos");
    strcpy(book_array[0].ISBN, "978-87-7681");
    strcpy(book_array[0].category, "Electrical engineering");
    book_array[0].copies = 10;
    book_array[0].current = 8;
    book_array[0].date.day = 22;
    book_array[0].date.month = 12;
    book_array[0].date.year = 2017;
    strcpy(book_array[0].publisher, "book boon");

    strcpy(book_array[1].title, "Concepts in Electric Circuits");
    strcpy(book_array[1].author, " Wasif Naem");
    strcpy(book_array[1].ISBN, "7681-499-1");
    strcpy(book_array[1].category, "Electrical engineering");
    book_array[1].copies = 20;
    book_array[1].current = 10;
    book_array[1].date.day = 21;
    book_array[1].date.month = 12;
    book_array[1].date.year = 2017;
    strcpy(book_array[1].publisher, "book boon");

    strcpy(book_array[2].title, "Engineering Thermodynamics");
    strcpy(book_array[2].author, "Tarik Shemmeri");
    strcpy(book_array[2].ISBN, "7681-670-4");
    strcpy(book_array[2].category, "Mechanical engineering");
    book_array[2].copies = 7;
    book_array[2].current = 5;
    book_array[2].date.day = 20;
    book_array[2].date.month = 12;
    book_array[2].date.year = 2017;
    strcpy(book_array[2].publisher, "book boon");
    strcpy(book_array[3].title, "Fundamentals of Construction Management");
    strcpy(book_array[3].author, "Dr. Abimbola Windapo");
    strcpy(book_array[3].ISBN, "403-0362-9");
    strcpy(book_array[3].category, "construction engineering");
    book_array[3].copies = 12;
    book_array[3].current = 11;
    book_array[3].date.day = 19;
    book_array[3].date.month = 11;
    book_array[3].date.year = 2017;
    strcpy(book_array[3].publisher, "Acair Ltd");

    return 4; // return the no. of filled items
}

void write_books(book a[], int items, const char *fileName)
{
    FILE *ptr = fopen(fileName, "wb");
    if (!ptr)
        return; // TODO: better error handling
    fwrite(a, sizeof(book), items, ptr);
    fclose(ptr);
}
int read_books(book a[], const char *fileName)
{
    FILE *ptr = fopen(fileName, "rb");
    if (!ptr)
        return 0;
    int n = 0;
    for (n = 0; !feof(ptr); ++n)
    {
        if (fread(&a[n], sizeof(book), 1, ptr) != 1)
            break;
    }

    fclose(ptr);
    return n;
}

int main()
{
    //    book book_array[100];
    //
    //    int items;
    //
    //    items = library_books(book_array);
    //
    //    if (items < 1)
    //        return -1;
    //
    //    write_books(book_array, items, "b.dat");
    //
    //    book *a = malloc(sizeof(book) * items);
    //    if (!a)
    //        return 0;
    //
    //    items = read_books(a, "b.dat");
    //
    //    printf("read %d books\n", items);
    //
    //    printf("%s\n", a[0].author);
    //    printf("%s\n", a[1].ISBN);
    //    printf("%d\n", a[2].date.day);
    //
    //    free(a);

    st_date item;

    FILE *ptr_data = fopen("test.txt", "wb");
    if (!ptr_data)
        return 1; // TODO: better error handling

    int count = 5;

    int i = 0;
    while (i < count)
    {
        sprintf(item.key, "mykey %d", i);
        sprintf(item.value, "myvalue кирилица %d", i);
        i++;
        fwrite(&item, sizeof(st_date), 1, ptr_data);
    }

    sprintf(item.key, "mykey %d", 100);
    sprintf(item.value, "myvalue кирилица %d", 100);
    fseek(ptr_data, 2 * sizeof(st_date), 0);
    fwrite(&item, sizeof(st_date), 1, ptr_data);

    fclose(ptr_data);

    ptr_data = fopen("test.txt", "rb");
    if (!ptr_data)
        return 0;
    int n = 0;
    int read_flag;
    for (n = 0; !feof(ptr_data); ++n)
    {
        read_flag = fread(&item, sizeof(st_date), 1, ptr_data);
        if (read_flag != 1)
        {
            break;
        }
        else
        {
            puts(item.value);
        }
    }

    //
    //    fclose(ptr);
    //    int max_n = 10;
    //    char *data = "Myc11";
    //    char cdata[max_n];
    //
    //    printf("size = %d \r\n", (int)sizeof(cdata));
    //
    //    int n = 0;
    //    while (n < max_n && data[n] != '\0')
    //    {
    //        cdata[n] = data[n];
    //        printf("=%d %c=", n, data[n]);
    //        n++;
    //    }
    //
    //    puts("");
    //
    //    printf("data = %s \r\n", cdata);

    return 0;
}