# Question 1

When parallelizing the outer for loop, count, i, and j should all be private. temp, a, and n should be shared. 

# Question 2

To parallelize the memcpy operation, we can copy the elements over one at a time in a for loop like this:

```
for (i = 0; i < n; i++)
{
    memcpy(a[i],temp[i],sizeof(int));
}
```

and then parallelize the for loop:

```
# pragma omp parallel for num_threads(thread_count) \
for (i = 0; i < n; i++)
{
    memcpy(a[i],temp[i],sizeof(int));
}
```

Instead of using memcpy, I will just use `std::vector` rather than a raw array, and copy memory element by element via `a[i] = temp[i]`. The reason I will do this is because the `std::vector` has a destructor that will free its memory automatically rather than needing to call `free()`, and because the `std::vector` has built in functionality that may be advantageous to me when writing this parallel program. 

# Question 3

See main.cpp