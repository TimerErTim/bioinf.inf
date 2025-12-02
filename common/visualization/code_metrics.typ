#import "@preview/lilaq:0.5.0" as lq

#let code-metrics-bar-graph(data) = {
  let _ = data.remove("Total")

  lq.diagram(
    width: 95%,
    height: 8cm,
    title: [Code-Metriken],
    xlabel: [Dateityp],
    ylabel: [Zeilen],
    xaxis: (
      ticks: data.keys()
      .map(rotate.with(-30deg, reflow: true))
      .enumerate(),
      subticks: none,
    ),


    lq.bar(
      range(data.len()),
      data.values().map(it => it.code),
      label: "Codezeilen",
      offset: -0.25,
      width: 0.25
    ),
    lq.bar(
      range(data.len()),
      data.values().map(it => it.comments),
      label: "Kommentarzeilen",
      width: 0.25
    ),
    lq.bar(
      range(data.len()),
      data.values().map(it => it.blanks),
      label: "Leerzeilen",
      offset: 0.25,
      width: 0.25
    ),
  )
}

#let big-o-n = {
  set text(fill: yellow)
  show math.equation: math.bold
  $O(n)$
}

#let big-o-1 = {
  set text(fill: green)
  show math.equation: math.bold
  $O(1)$
}

#let big-o-log-n = {
  set text(fill: green)
  show math.equation: math.bold
  $O(log(n))$
}

#let big-o-n-log-n = {
  set text(fill: orange)
  show math.equation: math.bold
  $O(n log(n))$
}

#let big-o-n-squared = {
  set text(fill: red)
  show math.equation: math.bold
  $O(n^2)$
}

#let big-o-n-cubed = {
  set text(fill: red)
  show math.equation: math.bold
  $O(n^3)$
}

#let big-o-2-to-the-n = {
  set text(fill: red)
  show math.equation: math.bold
  $O(2^n)$
}

#let big-o-n-factorial = {
  set text(fill: red)
  show math.equation: math.bold
  $O(n!)$
}
