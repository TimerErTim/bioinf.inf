 #import "@preview/fletcher:0.5.8" as fletcher: node, edge

#let visualize-doubly-linked-list-sentinel-structure() = {
  fletcher.diagram(
    node-corner-radius: 0pt,
    node-shape: rect,
      node(`head_`),
      node((4, 0), `tail_`),
      edge((0, 0), (0, 1), "-"),
      edge((4, 0), (4, 1), "-"),

      node((0, 1), [Sentinel\ head], stroke: 1pt),
      edge("-|>", shift: 1mm), edge("<|-", shift: -1mm),
      node((1, 1), [Node\ A], stroke: 1pt),
      edge("-|>", shift: 1mm), edge("<|-", shift: -1mm),
      node((2, 1), [Node\ B], stroke: 1pt),
      edge("-|>", shift: 1mm), edge("<|-", shift: -1mm),
      node((3, 1), [Node\ C], stroke: 1pt),
      edge("-|>", shift: 1mm), edge("<|-", shift: -1mm),
      node((4, 1), [Sentinel\ tail], stroke: 1pt),
  )
}
