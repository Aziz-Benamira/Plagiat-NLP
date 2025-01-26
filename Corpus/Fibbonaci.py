# Function to calculate Fibonacci series up to n terms
def fibonacci(n):
    fib_sequence = []
    a, b = 0, 1
    for _ in range(n):
        fib_sequence.append(a)
        a, b = b, a + b
    return fib_sequence

# Example usage
n_terms = 10
print(f"Fibonacci series up to {n_terms} terms: {fibonacci(n_terms)}")