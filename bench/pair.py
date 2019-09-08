from bench import bench

print(bench(1000000, '''
stack = []
''', '''
a, (b, (c, (d, (e, (f, g))))) = (1, (2, (3, (4, (5, (6, 7))))))
'''))
