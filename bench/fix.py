from bench import bench

print(bench(10, 'x = 0', '''
for _ in range(100000):
  x += 0.1
'''))
