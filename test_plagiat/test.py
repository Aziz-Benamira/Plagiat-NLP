# Function to compute Fibonacci sequence up to a given number of terms
def calculate_fib(num_terms):
    sequence = []
    x, y = 0, 1
    for i in range(num_terms):
        sequence.append(x)
        x, y = y, x + y
    return sequence

# Demonstration
terms = 10
print(f"Fibonacci sequence up to {terms} terms: {calculate_fib(terms)}")