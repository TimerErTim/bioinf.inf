#import "@preview/cetz:0.3.4": canvas, draw, tree

#let tree_graphic(data) = {
  canvas({
    import draw: *

    set-style(content: (padding: -2.5pt), fill: gray.lighten(70%), stroke: gray.lighten(70%))

    tree.tree(data, spread: 2.5, grow: 1.5, draw-node: (node, ..) => {
      circle((), radius: .45, stroke: none)
      content((), node.content)
    }, draw-edge: (from, to, ..) => {
      line((a: from, number: .6, b: to), (a: to, number: .6, b: from), mark: (end: ">"))
    }, name: "tree")
  })
}