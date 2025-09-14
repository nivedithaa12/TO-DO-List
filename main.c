/* todo.c
   Simple To-Do List Organizer in C
   Features:
   - Add task
   - List tasks
   - Mark task complete
   - Delete task
   - Save to / load from file (tasks.txt)
   - Autosave on exit
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LINE_MAX 256
#define FILE_NAME "tasks.txt"

typedef struct {
    int id;
    char desc[LINE_MAX];
    int done; // 0 = not done, 1 = done
} Task;

typedef struct {
    Task *arr;
    int size;
    int capacity;
    int next_id;
} TaskList;

/* ------ Utility functions ------ */

void init_list(TaskList *list) {
    list->capacity = 8;
    list->size = 0;
    list->arr = malloc(sizeof(Task) * list->capacity);
    list->next_id = 1;
}

void free_list(TaskList *list) {
    free(list->arr);
    list->arr = NULL;
    list->size = 0;
    list->capacity = 0;
}

void ensure_capacity(TaskList *list) {
    if (list->size >= list->capacity) {
        list->capacity *= 2;
        list->arr = realloc(list->arr, sizeof(Task) * list->capacity);
        if (!list->arr) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }
}

/* ------ Core operations ------ */

void add_task(TaskList *list, const char *desc) {
    ensure_capacity(list);
    Task *t = &list->arr[list->size++];
    t->id = list->next_id++;
    strncpy(t->desc, desc, LINE_MAX - 1);
    t->desc[LINE_MAX - 1] = '\0';
    t->done = 0;
    printf("Added task [%d]\n", t->id);
}

void list_tasks(const TaskList *list) {
    if (list->size == 0) {
        puts("No tasks yet.");
        return;
    }
    puts("ID  | Done | Description");
    puts("----+------+--------------------------------------");
    for (int i = 0; i < list->size; ++i) {
        const Task *t = &list->arr[i];
        printf("%-3d |  %c   | %s\n", t->id, (t->done ? 'X' : ' '), t->desc);
    }
}

Task *find_task_by_id(TaskList *list, int id) {
    for (int i = 0; i < list->size; ++i) {
        if (list->arr[i].id == id) return &list->arr[i];
    }
    return NULL;
}

void mark_done(TaskList *list, int id) {
    Task *t = find_task_by_id(list, id);
    if (!t) {
        printf("Task id %d not found.\n", id);
        return;
    }
    t->done = 1;
    printf("Marked task %d done.\n", id);
}

void delete_task(TaskList *list, int id) {
    int idx = -1;
    for (int i = 0; i < list->size; ++i) {
        if (list->arr[i].id == id) { idx = i; break; }
    }
    if (idx == -1) {
        printf("Task id %d not found.\n", id);
        return;
    }
    // shift left
    for (int i = idx; i < list->size - 1; ++i) {
        list->arr[i] = list->arr[i + 1];
    }
    list->size--;
    printf("Deleted task %d.\n", id);
}

/* ------ File persistence (simple text format) ------

File format (each line):
id|done|description
example:
1|0|Buy milk
*/

void save_tasks(const TaskList *list) {
    FILE *f = fopen(FILE_NAME, "w");
    if (!f) {
        perror("Saving tasks failed");
        return;
    }
    for (int i = 0; i < list->size; ++i) {
        const Task *t = &list->arr[i];
        // replace any newline chars in description
        char temp[LINE_MAX];
        strncpy(temp, t->desc, LINE_MAX);
        for (int j = 0; temp[j]; ++j) if (temp[j] == '\n') temp[j] = ' ';
        fprintf(f, "%d|%d|%s\n", t->id, t->done, temp);
    }
    fclose(f);
}

void load_tasks(TaskList *list) {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) return; // no file yet
    char line[LINE_MAX + 10];
    int max_id = 0;
    while (fgets(line, sizeof(line), f)) {
        // trim newline
        line[strcspn(line, "\r\n")] = 0;
        char *p1 = strchr(line, '|');
        if (!p1) continue;
        char *p2 = strchr(p1 + 1, '|');
        if (!p2) continue;
        *p1 = '\0';
        *p2 = '\0';
        int id = atoi(line);
        int done = atoi(p1 + 1);
        char *desc = p2 + 1;
        ensure_capacity(list);
        Task *t = &list->arr[list->size++];
        t->id = id;
        t->done = done ? 1 : 0;
        strncpy(t->desc, desc, LINE_MAX - 1);
        t->desc[LINE_MAX - 1] = '\0';
        if (id > max_id) max_id = id;
    }
    list->next_id = max_id + 1;
    fclose(f);
}

/* ------ Input helpers ------ */

void read_line(char *buf, int n) {
    if (!fgets(buf, n, stdin)) {
        buf[0] = '\0';
        return;
    }
    // remove trailing newline
    buf[strcspn(buf, "\r\n")] = 0;
}

int menu_choice() {
    char buf[32];
    printf("\nChoose: [a]dd  [l]ist  [m]ark done  [d]elete  [s]ave  [q]uit\n> ");
    read_line(buf, sizeof(buf));
    return buf[0] ? tolower((unsigned char)buf[0]) : '\0';
}

/* ------ Main ------ */

int main(void) {
    TaskList list;
    init_list(&list);
    load_tasks(&list);

    puts("=== To-Do List Organizer ===");
    char desc[LINE_MAX];

    while (1) {
        int ch = menu_choice();
        if (ch == 'a') {
            printf("Enter task description: ");
            read_line(desc, sizeof(desc));
            if (strlen(desc) == 0) {
                puts("Empty task not added.");
            } else {
                add_task(&list, desc);
            }
        } else if (ch == 'l') {
            list_tasks(&list);
        } else if (ch == 'm') {
            printf("Enter task id to mark done: ");
            char tmp[32]; read_line(tmp, sizeof(tmp));
            int id = atoi(tmp);
            if (id > 0) mark_done(&list, id);
            else puts("Invalid id.");
        } else if (ch == 'd') {
            printf("Enter task id to delete: ");
            char tmp[32]; read_line(tmp, sizeof(tmp));
            int id = atoi(tmp);
            if (id > 0) delete_task(&list, id);
            else puts("Invalid id.");
        } else if (ch == 's') {
            save_tasks(&list);
            puts("Saved.");
        } else if (ch == 'q') {
            save_tasks(&list);
            puts("Saved. Bye!");
            break;
        } else {
            puts("Unknown option. Try again.");
        }
    }

    free_list(&list);
    return 0;
}
