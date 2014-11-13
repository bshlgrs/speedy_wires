def line_world(n):
  print(n)
  print("0 0 0")
  for a in range(n):
    print("0 %d 2"%a)

line_world(10000)