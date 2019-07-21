from bench import bench

print(bench(10, '''
def fib_iter(n):
  a, b = 0, 1

  for _ in range(n - 1):
    a, b = b, a
    b += a

  return b
''', '''
for _ in range(10000):
  fib_iter(20)
'''))
