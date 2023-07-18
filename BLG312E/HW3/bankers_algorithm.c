#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_LINE_LENGTH 1000

void read_process_info(const char* filename, int*** process_values, int* rows, int* cols) {
    FILE *file;
    char line[MAX_LINE_LENGTH];
    file = fopen(filename, "r");
    char delimiter[] = " ";
    char *ptr;
    int i, j;
    int row_num = 0;
    int col_num = 0;
    bool is_col_counted = false;
    int value;

    if (file == NULL) {
        printf("Failed to open the file.\n");
        return;
    }

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        ptr = strtok(line, delimiter);
        row_num++;
        if (!is_col_counted)
            col_num = 0;
        while (ptr != NULL) {
            ptr = strtok(NULL, delimiter);
            if (!is_col_counted)
                col_num++;
        }
        is_col_counted = true;
    }


    rewind(file);

    *process_values = (int**)malloc(row_num * sizeof(int*));
    for (i = 0; i < row_num; i++) 
        (*process_values)[i] = (int*)malloc(col_num * sizeof(int));

    for (i = 0; i < row_num; i++) {
        for (j = 0; j < col_num; j++) {
            fscanf(file, "%d", &value);
            (*process_values)[i][j] = value;
        }
    }

    *rows = row_num;
    *cols = col_num;

    fclose(file);
}

void read_resource_info(const char* filename, int** resource_values, int* rows, int* cols) {
    FILE *file;
    char line[MAX_LINE_LENGTH];
    file = fopen(filename, "r");
    char delimiter[] = " ";
    int i, j;
    char *ptr;
    int row_num = 0;
    int col_num = 0;
    bool is_col_counted = false;
    int value;

    if (file == NULL) {
        printf("Failed to open the file.\n");
        return;
    }

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        ptr = strtok(line, delimiter);
        row_num++;
        if (!is_col_counted)
            col_num = 0;
        while (ptr != NULL) {
            ptr = strtok(NULL, delimiter);
            if (!is_col_counted)
                col_num++;
        }
        is_col_counted = true;
    }


    rewind(file);

    *resource_values = (int*)malloc(col_num * sizeof(int));

    for (j = 0; j < col_num; j++) {
        fscanf(file, "%d", &value);
        (*resource_values)[j] = value;
    }
    printf("\n");

    *rows = row_num;
    *cols = col_num;

    fclose(file);
}



void bankers_algorithm(int n, int m, int** resource_requests, int** resource_allocations, int* base_resources) {
    bool* flags = (bool*)malloc(n * sizeof(bool));
    int* available_resources = (int*)malloc(n * sizeof(int));
    int current_allocation;
    int i,j;
    // Initialize the arrays
    for (i = 0; i < n; i++) {
        flags[i] = false;
        available_resources[i] = 0;
    }

    for(i = 0; i < m; i++)
    {
        current_allocation = 0;
        for(j = 0; j < n; j++)
            current_allocation += resource_allocations[j][i];
        available_resources[i] = base_resources[i] - current_allocation;
    }
    // print process informations
    
    for (i = 0; i < n; i++) {
        printf("Information for process: P%d: \n", i+1);

        printf("Allocated resources: ");
        for (j = 0; j < m; j++)
            printf("R%d:%d ", j+1, resource_allocations[i][j]);

        printf("\nResource request : ");
        for (j = 0; j < m; j++)
            printf("R%d:%d ", j+1, resource_requests[i][j]);

        printf("\n\n");
    }

    int is_all_done_cnt = 0;
    int process_idx = 0;
    bool is_allocatable;
    int running_order[n];
    int order_idx=0;

    int deadlocks[n];
    int deadlock_idx = 0;
    for(i = 0; i < n; i++)
    {
        running_order[i] = 0;
        deadlocks[i] = 0;
    }
    // bankers algorithm to determine running order of processes
    // also to detect deadlocks
    while(is_all_done_cnt < n)
    {
        // check the process is already included in running order
        if (flags[process_idx] == false)
        {
            // allocatable check boolean
            is_allocatable = true;
            // for each resource compare request and available
            for(i=0; i < m; i++)
                if(resource_requests[process_idx][i] > available_resources[i])
                    is_allocatable = false;
            // if all requested resources are lower than allocatable
            if(is_allocatable)
            {
                // include process in running order   
                running_order[order_idx] = process_idx+1;
                // increase the running order counter index
                order_idx++;
                // for each resource load allocated resource of that process
                for(i=0; i < m; i++)
                    available_resources[i] += resource_allocations[process_idx][i];
                // reset counter since there might be new process that can be allocated
                is_all_done_cnt = 0;
                // set flag to true to not check this process again
                flags[process_idx] = true;
            }
            else
                is_all_done_cnt++;
        }
        // if it is included
        else
            is_all_done_cnt++;

        // iterate process index
        process_idx++;
        if(process_idx == n)
            process_idx = 0;
    }
    
    // define deadlocked processes
    for(i =0;i<n; i++)
    {
        if(flags[i]==false)
        {
            deadlocks[deadlock_idx] = i+1;
            deadlock_idx++;
        }
    }

    // print running order of processes
    if(running_order[0] != 0)
    {
        printf("Running order for processes: ");
        for(i = 0; i < n; i++)
        {
            if(running_order[i]==0)
                break;
            printf("P%d ",running_order[i]);
        }
        printf("\n");
    }

    // print deadlocks
    if(deadlocks[0] != 0)
    {
        printf("\nThere is a deadlock: ");
        for(i = 0; i < n; i++)
        {
            if(deadlocks[i]==0)
                break;
            printf("P%d ",deadlocks[i]);
        }
        printf("are the cause of deadlock.\n");
    }
    
    
        
    // Free allocated memories
    free(flags);
    free(available_resources);
    for (i = 0; i < n; i++) {
        free(resource_requests[i]);
        free(resource_allocations[i]);
    }
    free(resource_allocations);
    free(resource_requests);
}




// Structs to define resources and processes
struct process {
    int** values;
    int rows;
    int columns;
};

struct resource {
    int* values;
    int rows;
    int columns;
};

int main() {
    struct process allocations, requests;
    struct resource resources;
    // read requests
    read_process_info("requests.txt", &requests.values, &requests.rows, &requests.columns);

    // read allocations
    read_process_info("allocations.txt", &allocations.values, &allocations.rows, &allocations.columns);

    // read resources
    read_resource_info("resources.txt", &resources.values, &resources.rows, &resources.columns);

    bankers_algorithm(requests.rows, requests.columns, requests.values, allocations.values, resources.values);



    return 0;
}
