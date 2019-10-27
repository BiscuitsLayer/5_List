#include <cstdio>
#include <cstdlib>
#include <cstring>

//TODO ListOK
//TODO ListInit
//TODO ListDestruct

//TODO Pthreads
//global flags
//mutex

const int LIST_SIZE = 10;
const int NUM_SIZE = 3;
const int SERVICE = 118;
const int POISON = -666;

typedef int Elem_t;

typedef struct {
    Elem_t data = 0;
    size_t prev = 0, next = 0;
} List_it;

struct List_t {
    List_it *items = nullptr;
    size_t size = 0;
    size_t head = 0, tale = 0;
    size_t free = 1;
    size_t dump_count = 1;
};

size_t ListValSearch (List_t *list, Elem_t val);
size_t ListInsertBefore (List_t *list, Elem_t val_s, Elem_t val_i);
size_t ListInsertAfter (List_t *list, Elem_t val_s, Elem_t val_i);
void ListDelete (List_t *list, Elem_t val);
size_t ListInsertBegin (List_t *list, Elem_t val);
size_t ListInsertEnd (List_t *list, Elem_t val);
void ListPrint (List_t *list);
void ListDump (List_t *list);
//void ListSort (List_t *list);
void UserTest (List_t *list);

int main() {
    List_t list = {};
    list.items = (List_it *) calloc (LIST_SIZE, sizeof(List_t));

    list.items[0].data = POISON; //Служебная ячейка
    list.items[0].next = list.items[0].prev = 0; //Указывает сама на себя

    for (size_t idx = 1; idx < LIST_SIZE; ++idx) { //Заполняю список пустых ячеек
        list.items[idx].next = (idx + 1) % LIST_SIZE;
        list.items[idx].prev = (idx - 1) % LIST_SIZE;
    }

    UserTest(&list);
    return 0;
}

size_t ListValSearch(List_t *list, Elem_t val) {
    for (size_t idx = list->head; idx != 0; idx = list->items[idx].next) {
        if (list->items[idx].data == val)
            return idx;
    }
    return 0;
}

size_t ListInsertBefore(List_t *list, Elem_t val_s, Elem_t val_i) {
    //Помещаем значение в первую доступную свободную ячейку
    size_t cur = list->free;
    list->items[cur].data = val_i;
    list->free = list->items[cur].next;
    ++list->size;

    size_t idx_after = ListValSearch(list, val_s);
    if (!idx_after) {
        printf("Element %d not found\n", val_s);
        exit(1);
    }

    if (idx_after == list->head)
        list->head = cur;

    if (list->items[idx_after].prev != 0)
        list->items[list->items[idx_after].prev].next = cur;
    list->items[cur].prev = list->items[idx_after].prev;
    list->items[idx_after].prev = cur;
    list->items[cur].next = idx_after;
    return cur;
}

size_t ListInsertAfter(List_t *list, Elem_t val_s, Elem_t val_i) {
    //Помещаем значение в первую доступную свободную ячейку
    size_t cur = list->free;
    list->items[cur].data = val_i;
    list->free = list->items[cur].next; //Сдвигаем указател на свободную ячейку
    ++list->size; //Увеличиваем размер списка

    size_t idx_before = ListValSearch(list, val_s); //Ищем элемент, после которого должен стоять только что вставленный
    if (!idx_before) {
        printf("Element %d not found\n", val_s);
        exit(1);
    }

    if (idx_before == list->tale)
        list->tale = cur;

    if (list->items[idx_before].next != 0)
        list->items[list->items[idx_before].next].prev = cur; //Следующий элемент указывает на вставленный элемент
    list->items[cur].next = list->items[idx_before].next; //Указатель вставленного элемента свапает с указателем предыдущего элемента
    list->items[idx_before].next = cur; //Делаем чтобы предыдущий элемент указывал на только что вставленный
    list->items[cur].prev = idx_before;
    return cur;
}

void ListDelete(List_t *list, Elem_t val) {
    size_t cur = ListValSearch(list, val);
    if (list->items[cur].prev != 0) //Перезаписываем следующий элемент у предыдущего
        list->items[list->items[cur].prev].next = list->items[cur].next;
    if (list->items[cur].next != 0) //Перезаписываем предыдущий элемент у следующего
    list->items[list->items[cur].next].prev = list->items[cur].prev;
    --list->size; //Уменьшаем размер списка

    //Перемещаем голову и хвост
    if (list->head == cur)
        list->head = list->items[cur].next;
    else if (list->tale == cur)
        list->tale = list->items[cur].prev;

    //Вносим только что удалённый элемент в начало списка пустых элементов
    list->items[cur].next = list->free;
    list->free = cur;
}

size_t ListInsertBegin (List_t *list, Elem_t val) {
    size_t cur = list->free;
    list->items[cur].data = val;
    list->free = list->items[cur].next;
    ++list->size;

    list->items[cur].prev = 0;
    list->items[cur].next = list->head;
    if (list->head != 0)
        list->items[list->head].prev = cur;
    else //Если head == 0, то и tale == 0
        list->tale = cur;
    list->head = cur;
}

size_t ListInsertEnd (List_t *list, Elem_t val) {
    size_t cur = list->free;
    list->items[cur].data = val;
    list->free = list->items[cur].next;
    ++list->size;

    list->items[cur].next = 0;
    list->items[cur].prev = list->tale;
    if (list->tale != 0)
        list->items[list->tale].next = cur;
    else //Если tale == 0, то и head == 0
        list->head = cur;
    list->tale = cur;
}

void ListPrint(List_t *list) {
    printf ("This is list print:\n");
    for (size_t idx = list->head; idx != 0; idx = list->items[idx].next) {
        printf ("%d ", list->items[idx].data);
    }
    printf ("\n");
}

void ListDump (List_t *list) {
    printf ("-------");

    for (size_t idx = 0; idx < LIST_SIZE; ++idx)
        printf ("-----");
    printf ("\n");

    printf ("HEAD: %d, TALE: %d, SIZE: %d, FREE: %d\n", list->head, list->tale, list->size, list->free);

    printf ("-------");

    for (size_t idx = 0; idx < LIST_SIZE; ++idx)
        printf ("-----");
    printf ("\n");

    printf (" idx: ");
    for (size_t idx = 0; idx < LIST_SIZE; ++idx)
        printf ("%5zu", idx);
    printf ("\n");

    printf ("-------");

    for (size_t idx = 0; idx < LIST_SIZE; ++idx)
        printf ("-----");
    printf ("\n");

    printf ("prev: ");
    for (size_t idx = 0; idx < LIST_SIZE; ++idx)
        printf ("%5zu", list->items[idx].prev);
    printf ("\n");

    printf ("data: ");
    for (size_t idx = 0; idx < LIST_SIZE; ++idx)
        printf ("%5d", list->items[idx].data);
    printf ("\n");

    printf ("next: ");
    for (size_t idx = 0; idx < LIST_SIZE; ++idx)
        printf ("%5zu", list->items[idx].next);
    printf ("\n");

    printf ("\n");

    FILE *Dot = fopen ("../temp.dot", "w");
    fprintf (Dot, "digraph {\nnode [shape=\"box\"]\n"
                  "edge [style=\"dashed\" dir=\"both\"]\n"
                  "dirtype = \"both\"\n"
                  "rankdir=\"LR\"\n");

    for (size_t idx = list->head; idx != 0; idx = list->items[idx].next) {
        fprintf (Dot, "\telem_%zu [label=<<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"3\">\n"
                      "\t\t<TR><TD COLSPAN=\"2\" BGCOLOR=\"white\"> idx: %zu </TD></TR>\n"
                     "\t\t<TR><TD COLSPAN=\"2\" BGCOLOR=\"white\"> data: %d </TD></TR>\n"
                     "\t\t<TR><TD BGCOLOR=\"salmon\" PORT=\"prev\"> prev: %d </TD>\n"
                     "\t\t<TD BGCOLOR=\"lawngreen\" PORT=\"next\"> next: %d </TD></TR>\n"
                     "\t</TABLE>>]\n", idx, idx, list->items[idx].data,
                     list->items[idx].prev == 0 ? -1 : list->items[idx].prev,
                     list->items[idx].next == 0 ? -1 : list->items[idx].next);
    }

    fprintf (Dot, "\"data\" -> elem_%zu\n", list->head);

    for (size_t idx = list->head; list->items[idx].next != 0; idx = list->items[idx].next)
        fprintf (Dot, "elem_%zu -> elem_%zu\n", idx, list->items[idx].next);

    for (size_t idx = list->free; idx != 0; idx = list->items[idx].next) {
        fprintf (Dot, "\telem_free_%zu [label=<<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"3\">\n"
                      "\t\t<TR><TD COLSPAN=\"2\" BGCOLOR=\"aquamarine\"> idx: %zu </TD></TR>\n"
                      "\t\t<TR><TD COLSPAN=\"2\" BGCOLOR=\"aquamarine\"> data: %d </TD></TR>\n"
                      "\t\t<TR><TD BGCOLOR=\"salmon\" PORT=\"prev\"> prev: %d </TD>\n"
                      "\t\t<TD BGCOLOR=\"lawngreen\" PORT=\"next\"> next: %d </TD></TR>\n"
                      "\t</TABLE>>]\n", idx, idx, list->items[idx].data,
                 list->items[idx].prev == 0 ? -1 : list->items[idx].prev,
                 list->items[idx].next == 0 ? -1 : list->items[idx].next);
    }

    fprintf (Dot, "\"free\" -> elem_free_%zu\n", list->free);

    for (size_t idx = list->free; list->items[idx].next != 0; idx = list->items[idx].next)
        fprintf (Dot, "elem_free_%zu -> elem_free_%zu\n", idx, list->items[idx].next);

    fprintf (Dot, "}\n");
    fclose (Dot);

    char *command = (char *) calloc (NUM_SIZE + SERVICE, sizeof (char));
    sprintf (command, "dot -Tpng /home/biscuitslayer/CLionProjects/5_List/temp.dot "
                      "-o/home/biscuitslayer/CLionProjects/5_List/images/temp%zu.png", list->dump_count);
    if (list->dump_count == 1)
        system ("rm /home/biscuitslayer/CLionProjects/5_List/images/*");
    system (command);
    ++list->dump_count;
}

void ListSort (List_t *list) {

    Elem_t *sorted_list_data = (Elem_t *) calloc (list->size, sizeof (Elem_t));

    for (size_t idx = list->head, sorted_idx = 1; idx != 0; idx = list->items[idx].next, ++sorted_idx)
        sorted_list_data[sorted_idx] = list->items[idx].data;
    for (size_t idx = 1; idx < LIST_SIZE; ++idx) {
        list->items[idx].next = (idx + 1) % LIST_SIZE;
        list->items[idx].prev = (idx - 1) % LIST_SIZE;
        if (idx <= list->size)
            list->items[idx].data = sorted_list_data[idx];
        else
            list->items[idx].data = 0;
    }

    list->head = 1;
    list->tale = list->size;
    list->items[list->size].next = 0;
    list->free = list->size + 1;
}

void UserTest(List_t *list) {
    ListDump (list);
    ListInsertEnd (list, 50);
    ListDump (list);
    ListInsertAfter (list, 50, 60);
    ListDump (list);
    ListInsertBefore (list, 60, 55);
    ListDump (list);
    ListDelete (list, 60);
    ListDump (list);
    ListInsertAfter (list, 55, 59);
    ListDump (list);
    ListInsertEnd (list, 70);
    ListDump (list);
    ListDelete (list, 50);
    ListDump (list);
    ListInsertBegin (list, 51);
    ListDump (list);
    ListDump (list);
    ListInsertBegin (list, 50);
    ListDump (list);
    ListSort (list);
    ListDump (list);
}