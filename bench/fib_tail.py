from bench import bench

print(bench(10, '''
def fib(n, a, b):
  if n > 1: fib(n-1, b, a+b)
  return a if n == 0 else b
''', '''
for _ in range(10000):
  fib(20, 0, 1)
'''))
