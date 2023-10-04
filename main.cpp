#include <omp.h>
#include <iostream>
#include <vector>

void count_sort(std::vector<int>& a, int thread_count);

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
		count_sort(data,thread_count);
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

	#pragma omp parallel for num_threads(thread_count) \
		private(count,i,j)
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < n; ++j)
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
	#		pragma omp critical
			{
				temp[count] = a[i];
			}
		}
	#pragma omp barrier
	#pragma omp parallel for num_threads(thread_count) \
		private(i)
		for (int i = 0; i < n; ++i)
		{
			a[i] = temp[i];
		}
	
}