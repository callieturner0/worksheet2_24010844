// Functions from functions.c
extern int sum_of_three(int a, int b, int c);
extern int multiply_two(int a, int b);
extern int find_largest(int a, int b, int c);

// Stores the answers
volatile int result1;
volatile int result2;
volatile int result3;

// Main kernel function
void kernel_main(void)
{
    // Add three numbers
    result1 = sum_of_three(10, 20, 30);

    // Multiply two numbers
    result2 = multiply_two(5, 6);

    // Find the biggest number
    result3 = find_largest(15, 40, 25);

    // Keep the kernel running
    while (1)
    {
    }
}