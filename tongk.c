#include <stdio.h>
#include <math.h>
#include <mpi.h>

int n = 10, k = 32;
int arr[100] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int visited[100];
int count;

void backtrack(int current_sum, int target_sum, int current_index) {
    if (current_sum == target_sum) {
        printf("%d: { ", ++count);
        for (int i = 0; i < n; i++) {
            if (visited[i]) {
                printf("%d ", arr[i]);
            }
        }
        printf("}\n");
        return;
    }
    
    if (current_index == n)
        return;
    
    visited[current_index] = 1;
    backtrack(current_sum + arr[current_index], target_sum, current_index + 1);
    visited[current_index] = 0;
    backtrack(current_sum, target_sum, current_index + 1);
}

void find_subsets_with_fixed_elements(int k, int id, int elements_fixed_number) {
	
    for (int i = 0; i < n; i++) {
        visited[i] = 0;
    }
    int i = 0, current_sum = 0;
    while (i < elements_fixed_number){
    	if (id % 2 == 1){
    		visited[i] = 1
    		current_sum += arr[i];
		}
		id /= 2;
		i++;
	}
	
    count = 0;
    backtrack(current_sum, k, elements_fixed_number);
}

int main() {
	/*
    printf("Enter the number of elements (n): ");
    scanf("%d", &n);

    printf("Enter the elements:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    
	printf("Enter the target sum (k): ");
    scanf("%d", &k);
	*/
	int rank, num_procs;
	MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    
    int elements_fixed_number = 0;
    while (num_procs > pow(2, elements_fixed_number)){
    	elements_fixed_number++;
	}

    printf("Subsets with sum %d:\n", k);
    find_subsets_with_fixed_elements(k, rank, elements_fixed_number);
    if (rank + num_procs < pow(2, elements_fixed_number)){
    	find_subsets_with_fixed_elements(k, rank + num_procs, elements_fixed_number);	
	}

	MPI_Finalize();
    return 0;
}
