#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "hash.h"
#include "bitmap.h"
struct list lists[10];
struct hash hashes[10];
struct bitmap* bitmaps[10];

bool list_less(const struct list_elem* a, const struct list_elem* b, void* aux) {
    struct list_item* item1, * item2;
    item1 = list_entry(a, struct list_item, elem);
    item2 = list_entry(b, struct list_item, elem);
    return (item1->data) < (item2->data);
}

bool hash_less(const struct hash_elem* a, const struct hash_elem* b, void* aux) {
    struct hash_item* item1, * item2;
    item1 = hash_entry(a, struct hash_item, elem);
    item2 = hash_entry(b, struct hash_item, elem);
    return (item1->data) < (item2->data);
}

void hash_square(struct hash_elem* a, void* aux) {
    struct hash_item* item = hash_entry(a, struct hash_item, elem);
    int data = item->data;
    item->data = data * data;
}

void hash_triple(struct hash_elem* a, void* aux) {
    struct hash_item* item = hash_entry(a, struct hash_item, elem);
    int data = item->data;
    item->data = data * data * data;
}

void hash_destructor(struct hash_elem* a, void* aux) {
    list_remove(&(a->list_elem));
}

unsigned hash_func(const struct hash_elem* a, void* aux) {
    return hash_int((hash_entry(a, struct hash_item, elem))->data);
}

int main() {
    struct list_elem* elem, * elem_temp, * elem_temp1, * elem_temp2;
    struct list_item* item;
    struct hash_elem* h_elem, * h_elem_temp;
    struct hash_item* h_item;
    struct hash_iterator hash_ite;
    struct hash_item hi;
    int instr_count = 1, number, insert_pos, remove_pos, splice_pos, swap_pos, token_size;
    char instr[50], * comd[10], * token, command[50];
    size_t b, cnt;
    bool tf;
    while (1) {
        instr_count = 1;
        fgets(instr, sizeof(instr), stdin);
        sscanf(instr, "%s", command);
        token = strtok(instr, " ");
        comd[0] = token;
        while (token != NULL) {
            token = strtok(NULL, " ");
            comd[instr_count++] = token;
        }
        token_size = strlen(comd[instr_count-2]);
	comd[instr_count-2][token_size] = '\0';
	comd[instr_count-2][token_size-1] = '\0';
        elem = (struct list_elem*)malloc(sizeof(struct list_elem));
        h_elem = (struct hash_elem*)malloc(sizeof(struct hash_elem));
        //quit
        if (strcmp(command, "quit") == 0) break;
        //create
        if (strcmp(command, "create") == 0) {
            if (strcmp(comd[1], "list") == 0) {//list create
                number = atoi(&comd[2][4]);
                list_init(&lists[number]);
            }
            else if (strcmp(comd[1], "hashtable") == 0) {//hash create
                number = atoi(&comd[2][4]);
                hash_init(&hashes[number], hash_func, hash_less, NULL);
            }
            else if (strcmp(comd[1], "bitmap") == 0) {//bitmap create
                number = atoi(&comd[2][2]);
                b = atoi(&comd[3][0]);
                bitmaps[number] = bitmap_create(b);
            }
        }
        //dumpdata
        if (strcmp(command, "dumpdata") == 0) {
            if (strncmp(comd[1], "list", 4) == 0) {//dumpdata list
                number = atoi(&comd[1][4]);
                for (elem_temp = list_begin(&lists[number]); elem_temp != list_end(&lists[number]); elem_temp = list_next(elem_temp)) {
                    item = list_entry(elem_temp, struct list_item, elem);
                    printf("%d ", item->data);
                }
		printf("\n");
            }
            else if (strncmp(comd[1], "hash", 4) == 0) {//dumpdata hash
                number = atoi(&comd[1][4]);
                hash_first(&hash_ite, &hashes[number]);
                for (; hash_next(&hash_ite);) {
                    h_item = hash_entry(hash_cur(&hash_ite), struct hash_item, elem);
                    printf("%d ", h_item->data);
                }
		printf("\n");
            }
            else if (strncmp(comd[1], "bm", 2) == 0) {//dumpdata bitmap
                number = atoi(&comd[1][2]);
                b = bitmap_size(bitmaps[number]);
                for (cnt = 0; cnt < b; cnt++) printf("%d", bitmap_test(bitmaps[number], cnt));
                printf("\n");
            }
        }
        //delete
        if (strcmp(command, "delete") == 0) {
            if (strncmp(comd[1], "list", 4) == 0) {//delete list
                number = atoi(&comd[1][4]);
                for (elem_temp = list_begin(&lists[number]); elem_temp != list_end(&lists[number]); elem_temp = list_remove(elem_temp));
                list_init(&lists[number]);
            }
            else if (strncmp(comd[1], "hash", 4) == 0) {//delete hash
                number = atoi(&comd[1][4]);
                hash_clear(&hashes[number], hash_destructor);
            }
            else if (strncmp(comd[1], "bm", 2) == 0) {//delete bitmap
                number = atoi(&comd[1][2]);
                bitmap_destroy(bitmaps[number]);
            }
        }
        //List
        else if (strcmp(command, "list_push_back") == 0) {//list_push_back
            item = list_entry(elem, struct list_item, elem);
            item->data = atoi(&comd[2][0]);
            number = atoi(&comd[1][4]);
            list_push_back(&lists[number], elem);
        }
        else if (strcmp(command, "list_front") == 0) {//list_front
            number = atoi(&comd[1][4]);
            elem_temp = list_front(&lists[number]);
            item = list_entry(elem_temp, struct list_item, elem);
            printf("%d\n", item->data);
        }
        else if (strcmp(command, "list_back") == 0) {//list_back
            number = atoi(&comd[1][4]);
            elem_temp = list_back(&lists[number]);
            item = list_entry(elem_temp, struct list_item, elem);
            printf("%d\n", item->data);
        }
        else if (strcmp(command, "list_pop_back") == 0) {//list_pop_back
            number = atoi(&comd[1][4]);
            elem_temp = list_pop_back(&lists[number]);
        }
        else if (strcmp(command, "list_pop_front") == 0) {//list_pop_front
            number = atoi(&comd[1][4]);
            elem_temp = list_pop_front(&lists[number]);
        }
        else if (strcmp(command, "list_insert") == 0) {//list_insert
            item = list_entry(elem, struct list_item, elem);
            item->data = atoi(&comd[3][0]);
            number = atoi(&comd[1][4]);
            insert_pos = atoi(&comd[2][0]);
            elem_temp = list_begin(&lists[number]);
            while (insert_pos > 0) {
                elem_temp = list_next(elem_temp);
                insert_pos--;
            }
            list_insert(elem_temp, elem);
        }
        else if (strcmp(command, "list_insert_ordered") == 0) {//list_insert_ordered
            number = atoi(&comd[1][4]);
            item = list_entry(elem, struct list_item, elem);
            item->data = atoi(&comd[2][0]);
            list_insert_ordered(&lists[number], elem, list_less, NULL);
        }
        else if (strcmp(command, "list_empty") == 0) {//list_empty
            number = atoi(&comd[1][4]);
            if (list_empty(&lists[number])) printf("true\n");
            else printf("false\n");
        }
        else if (strcmp(command, "list_size") == 0) {//list_size
            number = atoi(&comd[1][4]);
            printf("%zu\n", list_size(&lists[number]));
        }
        else if (strcmp(command, "list_max") == 0) {//list_max
            number = atoi(&comd[1][4]);
            elem_temp = list_max(&lists[number], list_less, NULL);
            item = list_entry(elem_temp, struct list_item, elem);
            printf("%d\n", item->data);
        }
        else if (strcmp(command, "list_min") == 0) {//list_min
            number = atoi(&comd[1][4]);
            elem_temp = list_min(&lists[number], list_less, NULL);
            item = list_entry(elem_temp, struct list_item, elem);
            printf("%d\n", item->data);
        }
        else if (strcmp(command, "list_push_front") == 0) {//list_push_front
            number = atoi(&comd[1][4]);
            item = list_entry(elem, struct list_item, elem);
            item->data = atoi(&comd[2][0]);
            list_push_front(&lists[number], elem);
        }
        else if (strcmp(command, "list_remove") == 0) {//list_remove
            number = atoi(&comd[1][4]);
            remove_pos = atoi(&comd[2][0]);
            elem_temp = list_begin(&lists[number]);
            while (remove_pos > 0) {
                elem_temp = list_next(elem_temp);
                remove_pos--;
            }
            elem_temp = list_remove(elem_temp);
        }
        else if (strcmp(command, "list_reverse") == 0) {//list_reverse
            number = atoi(&comd[1][4]);
            list_reverse(&lists[number]);
        }
        else if (strcmp(command, "list_shuffle") == 0) {//list_shuffle
            number = atoi(&comd[1][4]);
            list_shuffle(&lists[number]);
        }
        else if (strcmp(command, "list_sort") == 0) {//list_sort
            number = atoi(&comd[1][4]);
            list_sort(&lists[number], list_less, NULL);
        }
        else if (strcmp(command, "list_splice") == 0) {//list_splice
            number = atoi(&comd[1][4]);
            splice_pos = atoi(&comd[2][0]);
            elem_temp = list_begin(&lists[number]);
            while (splice_pos-- > 0) elem_temp = list_next(elem_temp);

            number = atoi(&comd[3][4]);
            splice_pos = atoi(&comd[4][0]);
            elem_temp1 = list_begin(&lists[number]);
            while (splice_pos-- > 0) elem_temp1 = list_next(elem_temp1);

            splice_pos = atoi(&comd[5][0]);
            elem_temp2 = list_begin(&lists[number]);
            while (splice_pos-- > 0) elem_temp2 = list_next(elem_temp2);

            list_splice(elem_temp, elem_temp1, elem_temp2);
        }
        else if (strcmp(command, "list_swap") == 0) {//list_swap
            number = atoi(&comd[1][4]);
            swap_pos = atoi(&comd[2][0]);
            elem_temp = list_begin(&lists[number]);
            while (swap_pos-- > 0) elem_temp = list_next(elem_temp);

            swap_pos = atoi(&comd[3][0]);
            elem_temp1 = list_begin(&lists[number]);
            while (swap_pos-- > 0) elem_temp1 = list_next(elem_temp1);

            list_swap(elem_temp, elem_temp1);
        }
        else if (strcmp(command, "list_unique") == 0) {//list_unique
            number = atoi(&comd[1][4]);
            if (!comd[2]) list_unique(&lists[number], NULL, list_less, NULL);
            else list_unique(&lists[number], &lists[atoi(&comd[2][4])], list_less, NULL);
        }
        //hash
        else if (strcmp(command, "hash_insert") == 0) {//hash_insert
            number = atoi(&comd[1][4]);
            h_item = hash_entry(h_elem, struct hash_item, elem);
            h_item->data = atoi(&comd[2][0]);
            h_elem_temp = hash_insert(&hashes[number], h_elem);
        }
        else if (strcmp(command, "hash_apply") == 0) {//hash_apply
            number = atoi(&comd[1][4]);
            if (strcmp(comd[2], "square") == 0) hash_apply(&hashes[number], hash_square);
            else hash_apply(&hashes[number], hash_triple);
        }
        else if (strcmp(command, "hash_delete") == 0) {//hash_delete
            number = atoi(&comd[1][4]);
            hi.data = atoi(&comd[2][0]);
            h_elem_temp = hash_find(&hashes[number], &(hi.elem));
            if (h_elem_temp) hash_delete(&hashes[number], h_elem_temp);
        }
        else if (strcmp(command, "hash_empty") == 0) {//hash_empty
            number = atoi(&comd[1][4]);
            if (hash_empty(&hashes[number])) printf("true\n");
            else printf("false\n");
        }
        else if (strcmp(command, "hash_size") == 0) {//hash_size
            number = atoi(&comd[1][4]);
            printf("%zu\n", hash_size(&hashes[number]));
        }
        else if (strcmp(command, "hash_clear") == 0) {//hash_clear
            number = atoi(&comd[1][4]);
            hash_clear(&hashes[number], hash_destructor);
        }
        else if (strcmp(command, "hash_find") == 0) {//hash_find
            number = atoi(&comd[1][4]);
            hi.data = atoi(&comd[2][0]);
            h_elem_temp = hash_find(&hashes[number], &(hi.elem));
            if (h_elem_temp) printf("%d\n", hash_entry(h_elem_temp, struct hash_item, elem)->data);
        }
        else if (strcmp(command, "hash_replace") == 0) {//hash_replace
            number = atoi(&comd[1][4]);
            h_item = hash_entry(h_elem, struct hash_item, elem);
            h_item->data = atoi(&comd[2][0]);
            hash_replace(&hashes[number], h_elem);
        }
        //bitmap
        else if (strcmp(command, "bitmap_mark") == 0) {//bitmap_mark
            number = atoi(&comd[1][2]);
            b = atoi(&comd[2][0]);
            bitmap_set(bitmaps[number], b, true);
        }
        else if (strcmp(command, "bitmap_all") == 0) {//bitmap_all
            number = atoi(&comd[1][2]);
            b = atoi(&comd[2][0]);
            cnt  = atoi(&comd[3][0]);
            if (bitmap_all(bitmaps[number], b, cnt)) printf("true\n");
            else printf("false\n");
        }
        else if (strcmp(command, "bitmap_any") == 0) {//bitmap_any
            number = atoi(&comd[1][2]);
            b = atoi(&comd[2][0]);
            tf = atoi(&comd[3][0]);
            if (bitmap_any(bitmaps[number], b, tf)) printf("true\n");
            else printf("false\n");
        }
        else if (strcmp(command, "bitmap_contains") == 0) {//bitmap_contains
            number = atoi(&comd[1][2]);
            b = atoi(&comd[2][0]);
            cnt  = atoi(&comd[3][0]);
            if (strncmp(comd[4], "true", 4) == 0) {
                if (bitmap_contains(bitmaps[number], b, cnt, true)) printf("true\n");
                else printf("false\n");
            }
            else {
     	          if (bitmap_contains(bitmaps[number], b, cnt, false)) printf("true\n");
		  else printf("false\n");
	    }
        }
        else if (strcmp(command, "bitmap_count") == 0) {//bitmap_count
            number = atoi(&comd[1][2]);
            b = atoi(&comd[2][0]);
            cnt = atoi(&comd[3][0]);
            if (strcmp(comd[4], "true") == 0) printf("%zu\n", bitmap_count(bitmaps[number], b, cnt, true));
            else printf("%zu\n", bitmap_count(bitmaps[number], b, cnt, false));
        }
        else if (strcmp(command, "bitmap_dump") == 0) {//bitmap_dump
            number = atoi(&comd[1][2]);
            bitmap_dump(bitmaps[number]);
        }
        else if (strcmp(command, "bitmap_expand") == 0) {//bitmap_expand
            number = atoi(&comd[1][2]);
            b = atoi(&comd[2][0]);
            bitmaps[number] = bitmap_expand(bitmaps[number], b);
        }
        else if (strcmp(command, "bitmap_set_all") == 0) {//bitmap_set_all
            number = atoi(&comd[1][2]);
            if (strcmp(comd[2], "true") == 0) bitmap_set_all(bitmaps[number], true);
            else bitmap_set_all(bitmaps[number], false);
        }
        else if (strcmp(command, "bitmap_flip") == 0) {//bitmap_flip
            number = atoi(&comd[1][2]);
            b = atoi(&comd[2][0]);
            bitmap_flip(bitmaps[number], b);
        }
        else if (strcmp(command, "bitmap_none") == 0) {//bitmap_none
            number = atoi(&comd[1][2]);
            b = atoi(&comd[2][0]);
            tf = atoi(&comd[3][0]);
            if (bitmap_none(bitmaps[number], b, tf)) printf("true\n");
            else printf("false\n");
        }
        else if (strcmp(command, "bitmap_reset") == 0) {//bitmap_reset
            number = atoi(&comd[1][2]);
            b = atoi(&comd[2][0]);
            bitmap_reset(bitmaps[number], b);
        }
        else if (strcmp(command, "bitmap_scan_and_flip") == 0) {//bitmap_scan_and_flip
            number = atoi(&comd[1][2]);
            b = atoi(&comd[2][0]);
            cnt = atoi(&comd[3][0]);
            if (strcmp(comd[4], "true") == 0) printf("%zu\n", bitmap_scan_and_flip(bitmaps[number], b, cnt, true));
            else printf("%zu\n", bitmap_scan_and_flip(bitmaps[number], b, cnt, false));
        }
        else if (strcmp(command, "bitmap_scan") == 0) {//bitmap_scan
            number = atoi(&comd[1][2]);
            b = atoi(&comd[2][0]);
            cnt = atoi(&comd[3][0]);

            if (strcmp(comd[4], "true") == 0) printf("%zu\n", bitmap_scan(bitmaps[number], b, cnt, true));
            else printf("%zu\n", bitmap_scan(bitmaps[number], b, cnt, false));
        }
        else if (strcmp(command, "bitmap_set_all") == 0) {//bitmap_set_all
            number = atoi(&comd[1][2]);
            if (strcmp(comd[2], "true") == 0) bitmap_set_all(bitmaps[number], true);
            else bitmap_set_all(bitmaps[number], false);
        }
        else if (strcmp(command, "bitmap_set") == 0) {//bitmap_set
            number = atoi(&comd[1][2]);
            b = atoi(&comd[2][0]);
            if (strcmp(comd[3], "true") == 0) bitmap_set(bitmaps[number], b, true);
            else bitmap_set(bitmaps[number], b, false);
        }
        else if (strcmp(command, "bitmap_set_multiple") == 0) {//bitmap_set_multiple
            number = atoi(&comd[1][2]);
            b = atoi(&comd[2][0]);
            cnt = atoi(&comd[3][0]);
            if (strcmp(comd[4], "true")==0) bitmap_set_multiple(bitmaps[number], b, cnt, true);
            else bitmap_set_multiple(bitmaps[number], b, cnt, false);
        }
        else if (strcmp(command, "bitmap_size") == 0) {//bitmap_size
            number = atoi(&comd[1][2]);
            printf("%zu\n", bitmap_size(bitmaps[number]));
        }
        else if (strcmp(command, "bitmap_test") == 0) {//bitmap_test
            number = atoi(&comd[1][2]);
            b = atoi(&comd[2][0]);
            if (bitmap_test(bitmaps[number], b)) printf("true\n");
            else printf("false\n");
        }
    }
    return 0;
}

