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

#let box_list(
  entries,
  inset: 5pt,
  gap: 0pt,
  direction: "horizontal",
  square: true,
) = context {
  let split_entry(entry) = {
    if type(entry) == array {
      let (content, modifiers) = entry
      return ([#content], modifiers)
    }
    if type(entry) == dictionary {
      let (
        content: content_,
        ..modifiers
      ) = entry
      return ([#content_], modifiers)
    }
    return ([#entry], (:))
  }

  let contents = entries.map(it => split_entry(it).at(0))
  let single_box_width = contents.map(it => measure(it).width).fold(0pt, (a, b) => if a > b { a } else { b })
  let single_box_height = contents.map(it => measure(it).height).fold(0pt, (a, b) => if a > b { a } else { b })
  if square {
    single_box_width = calc.max(single_box_width, single_box_height)
    single_box_height = single_box_width
  }

  canvas({
    import draw: *

    for (i, entry) in entries.enumerate() {
      let (content_, modifiers) = split_entry(entry)

      let x = 0pt;
      let y = 0pt;

      if direction == "horizontal" {
        x = i * (single_box_width + 2*inset + gap);
      } else {
        y = i * (single_box_height + 2*inset + gap);
      }
      
      let x2 = x + 2 * inset + single_box_width;
      let y2 = y + 2 * inset + single_box_height;
      let xmid = (x + x2) / 2;
      let ymid = (y + y2) / 2;

      rect((x: x, y: y), (x: x2, y: y2), stroke: black, ..modifiers)
      content((xmid, ymid), content_, anchor: "center")
    }
  })
}

#let collect_by_key(list, key-fn) = {
  let dict = (:)
  for item in list {
    let key-value = str(key-fn(item))
    if key-value not in dict {
      dict.insert(key-value, ())
    }
    dict.at(key-value).push(item)
  }
  return dict
}

#let map_dict_values(dict_, map-fn) = {
  return dict_.pairs().map(((key, value)) => (key, map-fn(value))).to-dict()
}
