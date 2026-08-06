#include <stdio.h>
#include <stdlib.h>

void getInputs(int pid[], int bt[], int at[], int n, int getPr, int pr[]) {
    int i;
    for (i = 0; i < n; i++) {
        pid[i] = i + 1;
        printf("\nProcess %d:\n", i + 1);
        printf(" Arrival Time: ");
        scanf("%d", &at[i]);
        printf(" Burst Time: ");
        scanf("%d", &bt[i]);
        if (getPr) {
            printf(" Priority: ");
            scanf("%d", &pr[i]);
        }
    }
}

void fcfs(int *pid, int *bt, int *at, int n, int *wt, int *tat, int *ct_arr, float *awt, float *atat) {
    int i;
    int ct = 0, tot_wt = 0, tot_tat = 0;
    for (i = 0; i < n; i++) {
        if (ct < at[i]) ct = at[i];
        ct += bt[i];
        ct_arr[i] = ct;
        tat[i] = ct_arr[i] - at[i];
        wt[i] = tat[i] - bt[i];
        tot_wt += wt[i];
        tot_tat += tat[i];
    }
    *awt = (float)tot_wt / n;
    *atat = (float)tot_tat / n;
}

void sjfs(int *pid, int *bt, int *at, int n, int *wt, int *tat, int *ct_arr, float *awt, float *atat) {
    int i;
    int done[n], tot_wt = 0, tot_tat = 0, ct = 0, count = 0;
    for (i = 0; i < n; i++) done[i] = 0;
    while (count < n) {
        int idx = -1, min_bt = 1e9;
        for (i = 0; i < n; i++) {
            if (at[i] <= ct && !done[i] && bt[i] < min_bt) {
                min_bt = bt[i];
                idx = i;
            }
        }
        if (idx != -1) {
            ct += bt[idx];
            ct_arr[idx] = ct;
            tat[idx] = ct_arr[idx] - at[idx];
            wt[idx] = tat[idx] - bt[idx];
            tot_wt += wt[idx];
            tot_tat += tat[idx];
            done[idx] = 1;
            count++;
        } else {
            ct++;
        }
    }
    *awt = (float)tot_wt / n;
    *atat = (float)tot_tat / n;
}

void srtf(int *pid, int *bt, int *at, int n, int *wt, int *tat, int *ct_arr, float *awt, float *atat) {
    int i;
    int rmt[n], done[n], tot_wt = 0, tot_tat = 0, ct = 0, count = 0;
    for (i = 0; i < n; i++) { rmt[i] = bt[i]; done[i] = 0; }
    while (count < n) {
        int idx = -1, min_rmt = 1e9;
        for (i = 0; i < n; i++) {
            if (at[i] <= ct && !done[i] && rmt[i] < min_rmt && rmt[i] > 0) {
                min_rmt = rmt[i];
                idx = i;
            }
        }
        if (idx != -1) {
            rmt[idx]--;
            ct++;
            if (rmt[idx] == 0) {
                done[idx] = 1;
                count++;
                ct_arr[idx] = ct;
                tat[idx] = ct_arr[idx] - at[idx];
                wt[idx] = tat[idx] - bt[idx];
                tot_wt += wt[idx];
                tot_tat += tat[idx];
            }
        } else {
            ct++;
        }
    }
    *awt = (float)tot_wt / n;
    *atat = (float)tot_tat / n;
}

void prioritySchedule(int *pid, int *bt, int *at, int *pr, int n, int *wt, int *tat, int *ct_arr, float *awt, float *atat) {
    int done[n], tot_wt = 0, tot_tat = 0, ct = 0, count = 0;
    int i;
    for (i = 0; i < n; i++) done[i] = 0;
    while (count < n) {
        int idx = -1, hp = 1e9;
        for (i = 0; i < n; i++) {
            if (at[i] <= ct && !done[i] && pr[i] < hp) {
                hp = pr[i];
                idx = i;
            }
        }
        if (idx != -1) {
            ct += bt[idx];
            ct_arr[idx] = ct;
            tat[idx] = ct_arr[idx] - at[idx];
            wt[idx] = tat[idx] - bt[idx];
            tot_wt += wt[idx];
            tot_tat += tat[idx];
            done[idx] = 1;
            count++;
        } else {
            ct++;
        }
    }
    *awt = (float)tot_wt / n;
    *atat = (float)tot_tat / n;
}

void roundRobin(int *pid, int *bt, int *at, int tq, int n, int *wt, int *tat, int *ct_arr, float *awt, float *atat) {
    int i;
    int rmt[n], tot_wt = 0, tot_tat = 0, ct = 0, count = 0;
    for (i = 0; i < n; i++) rmt[i] = bt[i];
    while (count < n) {
        int active = 0;
        for (i = 0; i < n; i++) {
            if (at[i] <= ct && rmt[i] > 0) {
                active = 1;
                if (rmt[i] > tq) {
                    ct += tq;
                    rmt[i] -= tq;
                } else {
                    ct += rmt[i];
                    ct_arr[i] = ct;
                    tat[i] = ct_arr[i] - at[i];
                    wt[i] = tat[i] - bt[i];
                    tot_wt += wt[i];
                    tot_tat += tat[i];
                    rmt[i] = 0;
                    count++;
                }
            }
        }
        if (!active) ct++;
    }
    *awt = (float)tot_wt / n;
    *atat = (float)tot_tat / n;
}

void printTable(int *pid, int *at, int *bt, int *pr, int showPr, int *ct_arr, int *wt, int *tat, int n) {
    int i;
    printf("\n%-6s%-6s%-6s", "PID", "AT", "BT");
    if (showPr) printf("%-6s", "PR");
    printf("%-6s%-6s%-6s\n", "CT", "WT", "TAT");
    for (i = 0; i < n; i++) {
        printf("%-6d%-6d%-6d", pid[i], at[i], bt[i]);
        if (showPr) printf("%-6d", pr[i]);
        printf("%-6d%-6d%-6d\n", ct_arr[i], wt[i], tat[i]);
    }
}

int main() {
    int choice, n, tq;
    float awt = 0, atat = 0;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    int pid[n], bt[n], at[n], pr[n], ct_arr[n], wt[n], tat[n];
    while (1) {
        printf("\n=========================================\n");
        printf("          CPU SCHEDULING ALGORITHMS      \n");
        printf("=========================================\n");
        printf("1. FCFS\n2. SJF\n3. SRTF\n4. Priority\n5. Round Robin\n6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        if (choice == 6) break;
        switch (choice) {
            case 1:
                getInputs(pid, bt, at, n, 0, pr);
                fcfs(pid, bt, at, n, wt, tat, ct_arr, &awt, &atat);
                printTable(pid, at, bt, pr, 0, ct_arr, wt, tat, n);
                break;
            case 2:
                getInputs(pid, bt, at, n, 0, pr);
                sjfs(pid, bt, at, n, wt, tat, ct_arr, &awt, &atat);
                printTable(pid, at, bt, pr, 0, ct_arr, wt, tat, n);
                break;
            case 3:
                getInputs(pid, bt, at, n, 0, pr);
                srtf(pid, bt, at, n, wt, tat, ct_arr, &awt, &atat);
                printTable(pid, at, bt, pr, 0, ct_arr, wt, tat, n);
                break;
            case 4:
                getInputs(pid, bt, at, n, 1, pr);
                prioritySchedule(pid, bt, at, pr, n, wt, tat, ct_arr, &awt, &atat);
                printTable(pid, at, bt, pr, 1, ct_arr, wt, tat, n);
                break;
            case 5:
                getInputs(pid, bt, at, n, 0, pr);
                printf("Enter Time Quantum: ");
                scanf("%d", &tq);
                roundRobin(pid, bt, at, tq, n, wt, tat, ct_arr, &awt, &atat);
                printTable(pid, at, bt, pr, 0, ct_arr, wt, tat, n);
                break;
            default:
                printf("Invalid choice!\n");
                continue;
        }
        printf("\nAverage Waiting Time    : %.2f", awt);
        printf("\nAverage Turnaround Time : %.2f\n", atat);
    }
    return 0;
}
