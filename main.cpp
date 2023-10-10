#include <omp.h>
#include <iostream>
#include <vector>
#include <chrono>

void count_sort(std::vector<int>& a, int thread_count);
void serial_count_sort(std::vector<int>& a);
void print_vector(std::vector<int> a);

int main(int argc, char const* argv[])
{
	if (argc == 3)
	{
		int thread_count = atoi(argv[1]);
		int n = atoi(argv[2]);
		srand(100);
		std::vector<int> data;

		// Load vector with random float values
		for (int i = 0; i < n; i++)
		{
			data.push_back(rand());
		}
		std::vector<int> serial_data = data;
		//Print original data
		std::cout << "Random Data:" << std::endl;
		print_vector(data);
		//Sort data
		auto parallel_start = std::chrono::high_resolution_clock::now();
		count_sort(data,thread_count);
		auto parallel_end = std::chrono::high_resolution_clock::now();
		auto serial_start = std::chrono::high_resolution_clock::now();
		serial_count_sort(serial_data);
		auto serial_end = std::chrono::high_resolution_clock::now();

		auto parallel_time = std::chrono::duration_cast<std::chrono::microseconds>(parallel_end-parallel_start);
		auto serial_time = std::chrono::duration_cast<std::chrono::microseconds>(serial_end-serial_start);

		//Print new data
		std::cout << "Sorted Data:" << std::endl;
		print_vector(data);

		if (data == serial_data)
		{
			std::cout << "Serial Sorting and Parallel Sorting Match!" << std::endl;
		}
		
		//Print times
		std::cout << "Time taken for Serial Algorithm: " << serial_time.count() << std::endl;
		std::cout << "Time taken for Parallel Algorithm: " << parallel_time.count() << std::endl;

	}
	else {
		std::cout << "Err: Invalid number of command line arguments specified" << std::endl;
		return 1;
	}



	return 0;
}

void count_sort(std::vector<int>& a, int thread_count)
{

	size_t n = a.size();

	std::vector<int> temp(n,-1); // Initialize n-vector with all -1's

	int count = 0;
	int i = 0;
	int j = 0;

	// Parallelize the outer for loop using omp parallel for
	#pragma omp parallel for num_threads(thread_count) \
		shared(a,n,temp) private(count,i,j)
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{
				if (a[j] < a[i])
				{
					count++;
				}
				else if (a[j] == a[i] && j < i)
				{
					count++;
				}
			}
			// Define a critical section so there is no race condition on temp using omp critical
			#pragma omp critical
			{
				temp[count] = a[i];
				count = 0;
			}
		}
	// Define a barrier so that temp is fully sorted before proceeding using omp barrier
	#pragma omp barrier
	// Parallelize the copying of temp over to a (previously a memcpy) using omp parallel for
	#pragma omp parallel for num_threads(thread_count) \
		default(none) shared(a,temp,n) private(i)
		for (i = 0; i < n; ++i)
		{
			a[i] = temp[i];
		}
}

void serial_count_sort(std::vector<int>& a)
{
	int count;
	size_t n = a.size();
	std::vector<int> temp(n,-1); // Initialize n-vector with all -1's
	for (int i = 0; i < n; i++)
	{
		count = 0;
		for (int j = 0; j < n; j++)
		{
			if (a[j] < a[i])
			{
				count++;
			}
			else if (a[j] == a[i] && j < i)
			{
				count++;
			}
		}
		temp[count] = a[i];
	}
	a = temp;
}

void print_vector(std::vector<int> a)
{
	for (int i = 0; i < a.size(); ++i)
	{
		if (i == a.size()-1)
		{
			std::cout << a[i] << std::endl << std::endl;
		}
		else 
		{
			std::cout << a[i] << ", ";
		}
	}
}