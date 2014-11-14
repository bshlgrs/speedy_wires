
def line_world(n):
  print(n)
  print("0 0 0")
  for a in range(n):
    print("0 %d 2"%a)

def import_world(filename):
  from PIL import Image
  im = Image.open(filename)
  cells = []
  for y in range(im.size[1]):
    for x in range(im.size[0]):
      pixel = im.getpixel((x,y))
      if pixel == (0, 0, 255):
        cells.append("%d %d 0"%(x,y))
      elif pixel == (255, 0, 0):
        cells.append("%d %d 1"%(x,y))
      elif pixel in ((255, 255, 0), (255, 254, 0), (255, 253, 0)):
        cells.append("%d %d 2"%(x,y))
      elif pixel != (0, 0, 0):
        print("Invalid pixel at %d %d: %s"%(x,y, str(pixel)))
        exit(1)

  print(len(cells))
  for line in cells:
    print(line)

import_world("worlds/Langton's_Ant_11x11.bmp")
# line_world(10000)