// Adds three numbers together
int sum_of_three(int a, int b, int c)
{
    return a + b + c;
}

// Multiplies two numbers
int multiply_two(int a, int b)
{
    return a * b;
}

// Finds the biggest of three numbers
int find_largest(int a, int b, int c)
{
    int largest = a;

    if (b > largest)
        largest = b;

    if (c > largest)
        largest = c;

    return largest;
}