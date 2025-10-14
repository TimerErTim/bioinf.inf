

#let format-raw(it) = {
  show raw: set text(font: "JetBrains Mono", ligatures: false)

  let highlight-color = gray.lighten(90%)
  if it.block {
    block(
      stroke: highlight-color,
      fill: highlight-color.lighten(75%),
      radius: 5pt,
      width: 100%,
      inset: (top: 10pt, right: 20pt, bottom: 10pt, left: 10pt)
    )[
      #place(
        top + right, 
        dx: 15pt,
        dy: -5pt,
        text(size: 8pt, fill: gray.darken(50%))[
          #it.lang
        ]
      )
      #it
    ]
  } else {
    it
  }
}

#let format-quote(quote) = {
  let border-color = gray.lighten(50%)
  if quote.block {
    block(stroke: (
      x: border-color,
    ), radius: 8pt, inset: (y: 4pt))[
      #quote
    ]
  } else {
    quote
  }
}

#let format-image(it) = {
  box(stroke: gray, it)
}

#let documentation-template(
  title: "",
  semester-term: "",
  author: "",
  doc
) = {
  set page(numbering: "1 von 1", number-align: right, header: [
    #semester-term #h(1fr) #author
  ])
  set heading(numbering: "1.1.")
  show heading: set block(below: 1em, above: 1.25em)
  set text(font: "Roboto Serif", lang: "de")

  align(center)[
    #text(17pt)[*#title*]\
    #text(14pt)[#semester-term]

    #text(16pt)[#author]
  ]

  context{
    let show_outline = counter(page).final().first() > 5

    if show_outline [
      #show outline.entry: it => [
        #set text(size: 14pt - it.element.level * 1.5pt)
        #it
      ]
      #outline(title: "Inhaltsverzeichnis")
      #pagebreak()
    ] else {
      v(2em)
    }
  }

  show raw: format-raw
  show quote: format-quote
  show image: format-image
  show figure: set align(start)

  doc
}
