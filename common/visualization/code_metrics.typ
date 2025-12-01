#import "@preview/lilaq:0.5.0" as lq

#let code-metrics-bar-graph(data) = {
  let _ = data.remove("Total")

  lq.diagram(
    width: 95%,
    height: 10cm,
    title: [Code-Metriken],
    xlabel: [Dateityp],
    ylabel: [Zeilen],
    xaxis: (
      ticks: data.keys()
       .map(rotate.with(-45deg, reflow: true))
      .map(align.with(right)).enumerate(),
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
