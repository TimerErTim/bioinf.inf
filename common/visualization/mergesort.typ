#import "../components.typ": *

#import "@preview/cetz:0.3.4": canvas, draw, tree
#import "@preview/suiji:0.4.0": *
#import "@preview/lilaq:0.5.0" as lq

#let visualize_merge_step() = {
  let column(left, right, bottom) = {
    canvas({
      import draw: *

      content((0, 1), box_list(left))
      content((0, 0.5), [+])
      content((0, 0), box_list(right))
      content((0, - 0.5), text(size: 7pt)[#sym.arrow.b])
      content((0, - 1), box_list(bottom))
    })
  }

  canvas({
    import draw: *

    let flat_to_tree_data(list_) = {
      if list_.len() == 1 {
        return ((data: list_.at(0)),)
      }

      let (first, ..others) = list_
      return ((data: first), flat_to_tree_data(others))
    }

    let tree_data = flat_to_tree_data(
      (
        (
          ((1, (stroke: red)), 8),
          ((2, (stroke: orange)), 5),
          (text(fill: white.opacify(100%))[0],) * 4
        ),
        (
          ((1, (fill: gray.lighten(70%))), (8, (stroke: orange))),
          ((2, (stroke: red)), 5),
          (1,) + (text(fill: white.opacify(100%))[0],) * 3
        ),
        (
          ((1, (fill: gray.lighten(70%))), (8, (stroke: orange))),
          ((2, (fill: gray.lighten(70%))), (5, (stroke: red))),
          (1, 2) + (text(fill: white.opacify(100%))[0],) * 2
        ),
        (
          ((1, (fill: gray.lighten(70%))), (8, (stroke: red))),
          ((2, (fill: gray.lighten(70%))), (5, (fill: gray.lighten(70%)))),
          (1, 2, 5) + (text(fill: white.opacify(100%))[0],)
        ),
        (
          ((1, (fill: gray.lighten(70%))), (8, (fill: gray.lighten(70%)))),
          ((2, (fill: gray.lighten(70%))), (5, (fill: gray.lighten(70%)))),
          (1, 2, 5, 8)
        ),
      )
    )

    tree.tree(
      tree_data,
      direction: "right",
      grow: 3,
      draw-node: (node, ..) => {
        content((), column(..node.content.data))
      },
      draw-edge: (from, to, ..) => {
        line((a: from, number: 1, b: to), (a: to, number: 1, b: from), mark: (end: ">"))
      }
    )
  })
  
  [
    #box(box_list((([], (stroke: red)),))) ... kleineres Element wird gewählt #h(2em)
    #box(box_list((([], (stroke: orange)),))) ... anderes Element als Kandidat
    #place(right + bottom, text(size: 0.5pt)[Fuck you, Sven!])
  ]
   
}

#let visualize_mergesort(data) = {
  let rng = gen-rng-f(43)

  let split_data(data, rng) = {
    if data.len() == 1 {
      return (data: data)
    }

    let left = ()
    let right = ()
    let left_count = data.len() / 2
    let right_count = data.len() - left_count

    let decider = 0;
    for entry in data {
      if left.len() == left_count {
        right.push(entry)
      } else if right.len() == right_count {
        left.push(entry)
      } else {
        // Distribute randomly
        (rng, decider) = random-f(rng)
        if decider < 0.5 {
          left.push(entry)
        } else {
          right.push(entry)
        }
      }
    }

    return ((data: data), split_data(left, rng), split_data(right, rng))
  }

  canvas(
    {
      import draw: *

      let tree_data = split_data(data, rng)

      tree.tree(tree_data, direction: "up", spread: 1, grow: 2, draw-node: (node, ..) => {
        content((), box_list(node.content.data.map(it => [#it])))
      }, draw-edge: (from, to, ..) => {
        line((a: from, number: 0.5, b: to), (a: to, number: 0.5, b: from), mark: (start: (symbol: ">", fill: black)))
      })
    },
  )
}

#let visualize_mergesort_complexity() = {
  lq.diagram(
    width: 75%,
    height: 8cm,
    title: [Komplexität des Merge Sort Algorithmus],
    xlabel: [Mengengröße $n$],
    ylabel: [Komplexität],
    xaxis: (format-ticks: none),
    yaxis: (format-ticks: none),
    xlim: (1, auto),
    ylim: (1, auto),
    lq.plot(range(1, 100), n => n * calc.log(n) + 1, label: [$T(n) = O(n log(n))$], mark: none),
    lq.plot(range(1, 100), n => n, label: [$S(n) = O(n)$], mark: none),
  )
}

#let visualize_mergesort_benchmarks_results(results) = {
  let data-dict = map_dict_values(collect_by_key(results.entries, it => it.mode), it => collect_by_key(it, it => it.len))

  let time-plots = (
    data-dict.in_memory.pairs().map(((size, data)) => lq.plot(data.map(it => it.n), data.map(it => it.elapsed_ms / 1000), label: [in-memory $"len"=#size$], mark: "^")) 
    +
    data-dict.on_disk.pairs().map(((size, data)) => lq.plot(data.map(it => it.n), data.map(it => it.elapsed_ms / 1000), label: [on-disk $"len"=#size$], mark: "s"))
  )

  let space-plots = (
    data-dict.in_memory.pairs().map(((size, data)) => lq.plot(data.map(it => it.n), data.map(it => it.peak_mem_kb / 1024), label: [in-memory $"len"=#size$], mark: "^")) 
    +
    data-dict.on_disk.pairs().map(((size, data)) => lq.plot(data.map(it => it.n), data.map(it => it.peak_mem_kb / 1024), label: [on-disk $"len"=#size$], mark: "s"))
  )

  block[
    #lq.diagram(
      width: 100%,
      height: 8cm,
      title: [$T(n)$ und $S(n)$ des Merge Sort Algorithmus mit größer werdender Datenmenge $n$],
      ylabel: [Laufzeit in $s$],
      xaxis: (mirror: false),
      yaxis: (mirror: false),

      ..time-plots,
    )
    #lq.diagram(
      width: 100%,
      height: 8cm,
      xaxis: (mirror: false),
      yaxis: (mirror: false),
      xlabel: [Datenmenge $n$],
      ylabel: [Speicherbedarf in $"MB"$ USS],
      legend: none,

      ..space-plots
    )
  ]
}
