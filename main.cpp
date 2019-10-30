#include "list_parts/ListLib.h"

int main() {
    List_t list = ListInit (LIST_SIZE);
    ListPushFront (&list, 20);
    ListPushFront (&list, 10);
    ListPushBack (&list, 30);
    ListPushAfter (&list, 30, 40);
    ListPushBack (&list, 60);
    ListPushBefore (&list, 60, 50);
    ListPushBack (&list, 70);
    ListPushBack (&list, 80);
    ListPushBack (&list, 90);
    ListPushBack (&list, 100);

    size_t idx = ListIdxSearch (&list, 3);
    printf ("FOUND %d\n", idx);
    ListSort (&list);

    size_t idx1 = ListIdxSearch (&list, 3);
    printf ("FOUND %d\n", idx1);

    ListDestruct (&list);
    return 0;
}