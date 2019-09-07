from bench import bench

print(bench(10000000, '''
stack = []
''', '''
stack.append((1, (2, (3, (4, (5, (6, 7)))))))
stack.pop();
'''))
