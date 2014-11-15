def coords_to_neighbor_list(coords_list):
  result = []
  for (x1, y1) in coords_list:
    neighbors = []
    for neighbor, (x2, y2) in enumerate(coords_list):
      if abs(x1 - x2) <= 1 and abs(y1 - y2) <= 1 and (x1,y1) != (x2,y2):
        neighbors.append(neighbor)
    result.append(neighbors)
  return result

def decompose(neighbor_list):
  labels = {}
  current_label = 0
  segments = []

  # Kind of dodgy bit: At the start, segments is a list of tuples of type
  # (type, list of nodes in the segment, list of neighboring nodes)

  # At the end, it is
  # (type, list of nodes in the segment, list of neighboring sections)

  for node, neighbors in enumerate(neighbor_list):
    if node in labels:
      continue

    # otherwise, we're grabbing a new section!
    this_section = []

    if len(neighbors) <= 2:
      this_section, neighbor_cells = get_segment(neighbor_list, node, True)
      segments.append(("simple", this_section, neighbor_cells))
    else:
      this_section, neighbor_cells = get_segment(neighbor_list, node, False)
      segments.append(("complex", this_section, neighbor_cells))

    for other_node in this_section:
      labels[other_node] = current_label

    current_label += 1

  result = []

  for (simple_or_complex, nodes, neighbor_nodes) in segments:
    result.append((simple_or_complex, nodes, set(labels[x] for x in neighbor_nodes)))

  return result

def get_segment(neighbor_list, node, simple):
  stack = [node]
  this_section = set()
  neighbor_cells = set()

  while stack:
    node = stack.pop()

    if node in this_section or node in neighbor_cells:
      continue

    # if this is in the same segment
    if (len(neighbor_list[node]) == 2) == simple or len(neighbor_list[node]) == 1:
      this_section.add(node)

      for neighbor in neighbor_list[node]:
        stack.append(neighbor)

    else:
      neighbor_cells.add(node)

  not_actual_neighbors = set()

  for potential_neighbor in neighbor_cells:
    if all(x in this_section for x in neighbor_list[potential_neighbor]):
      not_actual_neighbors.add(potential_neighbor)

  this_section |= not_actual_neighbors
  neighbor_cells ^= not_actual_neighbors
  return this_section, neighbor_cells

print decompose(coords_to_neighbor_list([(0,0),(1,0),(2,0),(3,0)]))
#                 # 0   1       2    3    4     5       6       7      8
# print decompose([[1],[0,2],[1,3],[2,4],[3,5],[4,6,7],[5,7,8],[5,6,8],[6,7]])