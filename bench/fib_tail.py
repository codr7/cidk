from bench import bench

print(bench(10, '''
def fib(n, a, b):
  if n == 0: return a
  if n == 1: return b
  return fib(n-1, b, a+b)
''', '''
for _ in range(10000):
  fib(20, 0, 1)
'''))
