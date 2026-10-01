#include <stdio.h>
#include <stdlib.h>

int detectSafety(int m, int n, int avail[n],
                 int max[m][n], int alloc[m][n])
{
    int work[n];
    int finish[m];
    int safe[m];

    int i, j;
    int count = 0;
    int found;

    for(j = 0; j < n; j++)
    {
        work[j] = avail[j];
    }

    for(i = 0; i < m; i++)
    {
        finish[i] = 0;
    }

    while(count < m)
    {
        found = 0;

        for(i = 0; i < m; i++)
        {
            if(finish[i] == 0)
            {
                int possible = 1;

                for(j = 0; j < n; j++)
                {
                    if(max[i][j] - alloc[i][j] > work[j])
                    {
                        possible = 0;
                        break;
                    }
                }

                if(possible)
                {
                    for(j = 0; j < n; j++)
                    {
                        work[j] = work[j] + alloc[i][j];
                    }

                    safe[count] = i;

                    finish[i] = 1;

                    count++;

                    found = 1;
                }
            }
        }

        if(found == 0)
        {
            break;
        }
    }

    if(count == m)
    {
        printf("Safe Sequence: ");

        for(i = 0; i < m; i++)
        {
            printf("P%d", safe[i]);

            if(i != m - 1)
            {
                printf(" -> ");
            }
        }

        printf("\n");

        return 1;
    }


    return 0;
}

int processResourceReq(int m, int n, int p, int req[n],
                       int avail[n], int max[m][n],
                       int alloc[m][n])
{
    int j;

    for(j = 0; j < n; j++)
    {
        if(req[j] > max[p][j] - alloc[p][j])
        {
            printf("\nError: Request exceeds maximum need.\n");
            printf("System is NOT SAFE for this request.\n");

            return 0;
        }
    }

    for(j = 0; j < n; j++)
    {
        if(req[j] > avail[j])
        {
            printf("\nResources are not available.\n");
            printf("System is NOT SAFE for this request.\n");

            return 0;
        }
    }

    for(j = 0; j < n; j++)
    {
        avail[j] = avail[j] - req[j];

        alloc[p][j] = alloc[p][j] + req[j];
    }


    printf("\nChecking Safety After Granting Request...\n");

    if(detectSafety(m, n, avail, max, alloc))
    {
        printf("System is SAFE after granting the request.\n");

        return 1;
    }

    printf("System is NOT SAFE after granting the request.\n");

    printf("Rolling back the request...\n");


    for(j = 0; j < n; j++)
    {
        avail[j] = avail[j] + req[j];

        alloc[p][j] = alloc[p][j] - req[j];
    }

    return 0;
}

int main()
{
    int m, n, i, j;
    int p;

    printf("Enter number of processes: ");
    scanf("%d", &m);

    printf("Enter number of resources: ");
    scanf("%d", &n);

    int alloc[m][n];
    int max[m][n];
    int avail[n];
    int req[n];

    printf("\nEnter Allocation Matrix:\n");

    for(i = 0; i < m; i++)
    {
        for(j = 0; j < n; j++)
        {
            scanf("%d", &alloc[i][j]);
        }
    }

    printf("\nEnter Max Matrix:\n");

    for(i = 0; i < m; i++)
    {
        for(j = 0; j < n; j++)
        {
            scanf("%d", &max[i][j]);
        }
    }

    printf("\nEnter Available Resources:\n");

    for(j = 0; j < n; j++)
    {
        scanf("%d", &avail[j]);
    }

    printf("\nNeed Matrix:\n");

    for(i = 0; i < m; i++)
    {
        for(j = 0; j < n; j++)
        {
            printf("%d ", max[i][j] - alloc[i][j]);
        }

        printf("\n");
    }

    printf("\nChecking Initial System Safety...\n");

    if(detectSafety(m, n, avail, max, alloc))
    {
        printf("System is SAFE.\n");
    }
    else
    {
        printf("System is NOT SAFE.\n");
    }

    printf("\nEnter process number making request: ");
    scanf("%d", &p);

    if(p < 0 || p >= m)
    {
        printf("Invalid process number.\n");
        return 0;
    }

    printf("Enter Resource Request for P%d:\n", p);

    for(j = 0; j < n; j++)
    {
        scanf("%d", &req[j]);
    }

    if(processResourceReq(m, n, p, req,
                          avail, max, alloc))
    {
        printf("Request can be GRANTED.\n");
    }
    else
    {
        printf("Request cannot be GRANTED.\n");
    }


    return 0;
}