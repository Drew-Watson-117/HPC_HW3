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

# Question 3

See main.cpp