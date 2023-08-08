#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int n= 33, k = 7;
int arr[100] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
		21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
		31, 32, 33, 34, 35, 36, 37, 38, 39, 40};
int visited[100];
int count;

int pow_int(int x, int y){
	int response = 1;
	for (int i=0; i<y; i++){
		response *= x;
	}
	return response;
}

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
    		visited[i] = 1;
    		current_sum += arr[i];
	}
	id /= 2;
	i++;
    }
	
    count = 0;
    backtrack(current_sum, k, elements_fixed_number);
}

int main(int argc, char * argv[]) {
	int rank, num_procs;
	double start, end;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	if (argc == 2){
		n = atoi(argv[1]);
	}
/*
	if (rank == 0){	
		printf("Enter the number of elements (n): ");
		scanf("%d", &n);
		printf("Enter the target sum (k): ");
		scanf("%d", &k);
	}
	
	printf("Enter the elements:\n");
	for (int i = 0; i < n; i++) {
		scanf("%d", &arr[i]);
	}
	*/

	if (rank == 0){
		start = MPI_Wtime();
	}
	int elements_fixed_number = 0;
	while (num_procs > pow_int(2, elements_fixed_number)){
		elements_fixed_number++;
	}

	printf("Subsets with sum %d:\n", k);
	find_subsets_with_fixed_elements(k, rank, elements_fixed_number);
	if (rank + num_procs < pow_int(2, elements_fixed_number)){
		find_subsets_with_fixed_elements(k, rank + num_procs, elements_fixed_number);	
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if (rank == 0){
		end = MPI_Wtime();
		printf("Start:%f\nEnd:%f\nDuration:%f\n", start, end, end-start);
	}
	MPI_Finalize();
	return 0;
}
