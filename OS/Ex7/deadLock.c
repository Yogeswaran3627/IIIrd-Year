#include <stdio.h>

#define MAX 100

struct Block {
    int size;
};

struct Process {
    int id;
    int size;
};

void printMemory(struct Block blocks[], int m)
{
    printf("Memory: ");

    for (int i = 0; i < m; i++) {
        printf("B%d=%d ", i + 1, blocks[i].size);
    }

    printf("\n");
}

void firstFit(struct Block blocks[], int m, struct Process p[], int n)
{
    printf("\n========== FIRST FIT ==========\n");

    for (int i = 0; i < n; i++) {
        int allocated = 0;

        for (int j = 0; j < m; j++) {
            if (blocks[j].size >= p[i].size) {
                blocks[j].size = blocks[j].size - p[i].size;

                printf("P%d (%d) -> B%d\n",
                       p[i].id, p[i].size, j + 1);

                printf("Remaining in B%d = %d\n",
                       j + 1, blocks[j].size);

                allocated = 1;
                break;
            }
        }

        if (allocated == 0) {
            printf("P%d (%d) -> Not Allocated\n",
                   p[i].id, p[i].size);
        }

        printMemory(blocks, m);
        printf("\n");
    }
}

void bestFit(struct Block blocks[], int m, struct Process p[], int n)
{
    printf("\n========== BEST FIT ==========\n");

    for (int i = 0; i < n; i++) {
        int best = -1;

        for (int j = 0; j < m; j++) {
            if (blocks[j].size >= p[i].size) {
                if (best == -1 ||
                    blocks[j].size < blocks[best].size) {
                    best = j;
                }
            }
        }

        if (best != -1) {
            blocks[best].size =
                blocks[best].size - p[i].size;

            printf("P%d (%d) -> B%d\n",
                   p[i].id, p[i].size, best + 1);

            printf("Remaining in B%d = %d\n",
                   best + 1, blocks[best].size);
        } else {
            printf("P%d (%d) -> Not Allocated\n",
                   p[i].id, p[i].size);
        }

        printMemory(blocks, m);
        printf("\n");
    }
}

void worstFit(struct Block blocks[], int m, struct Process p[], int n)
{
    printf("\n========== WORST FIT ==========\n");

    for (int i = 0; i < n; i++) {
        int worst = -1;

        for (int j = 0; j < m; j++) {
            if (blocks[j].size >= p[i].size) {
                if (worst == -1 ||
                    blocks[j].size > blocks[worst].size) {
                    worst = j;
                }
            }
        }

        if (worst != -1) {
            blocks[worst].size =
                blocks[worst].size - p[i].size;

            printf("P%d (%d) -> B%d\n",
                   p[i].id, p[i].size, worst + 1);

            printf("Remaining in B%d = %d\n",
                   worst + 1, blocks[worst].size);
        } else {
            printf("P%d (%d) -> Not Allocated\n",
                   p[i].id, p[i].size);
        }

        printMemory(blocks, m);
        printf("\n");
    }
}

int main()
{
    struct Block blocks[MAX];
    struct Process p[MAX];

    int m, n;

    printf("Enter number of Memory Blocks: ");
    scanf("%d", &m);

    printf("Enter sizes of Memory Blocks:\n");

    for (int i = 0; i < m; i++) {
        printf("Block %d: ", i + 1);
        scanf("%d", &blocks[i].size);
    }

    printf("\nEnter number of Processes: ");
    scanf("%d", &n);

    printf("Enter sizes of Processes:\n");

    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;

        printf("Process %d: ", i + 1);
        scanf("%d", &p[i].size);
    }

    struct Block first[MAX];
    struct Block best[MAX];
    struct Block worst[MAX];

    for (int i = 0; i < m; i++) {
        first[i] = blocks[i];
        best[i] = blocks[i];
        worst[i] = blocks[i];
    }

    firstFit(first, m, p, n);
    bestFit(best, m, p, n);
    worstFit(worst, m, p, n);

    return 0;
}