#let table-test-results-overview(data) = {
  set table(fill: (x, y) => { if y == 0 { gray.lighten(75%) } })

  let entry(name, tests, disabled, failures, errors) = {
    let tests = tests - disabled
    let failures = failures + errors
    let color = if failures > 0 { red.lighten(25%) } else { green.lighten(25%) }

    (table.cell(fill: color, name), table.cell(fill: color)[#tests], table.cell(fill: color)[#failures])
  }

  table(
    columns: 3,
    table.header[*Testsuite*][*Tests*][*Fehler*],
    ..entry([*Total*], data.tests, data.disabled, data.failures, data.errors),
    table.hline(start: 0, end: 3, stroke: 2pt),
    ..for testsuite in data.testsuites {
      entry(testsuite.name, testsuite.tests, testsuite.disabled, testsuite.failures, testsuite.errors)
    },
  )
}

#let table-test-results-detailed(data) = {
  set table(fill: (x, y) => { if y == 0 { gray.lighten(75%) } })

  let entry(testcase) = {
    let is_disabled = testcase.status != "RUN"
    let has_failed = "failures" in testcase
    let has_errors = "errors" in testcase

    let color = if is_disabled { gray.lighten(50%) } else {
      if has_failed or has_errors { red.lighten(25%) } else { green.lighten(25%) }
    }
    let symbol = if is_disabled { sym.quest } else { if has_failed or has_errors { sym.crossmark } else { sym.checkmark } }

    (table.cell(fill: color, inset: (left: 0.5cm), align: horizon)[
      #set text(lang: "en", hyphenate: true, overhang: false, size: 10pt)
      #layout(size => {
        let display_text = testcase.name
        let truncated = false
        while measure(display_text + if truncated { "..."}).width > size.width {
          display_text = display_text.slice(0, -1)
          truncated = true
        }
        return display_text + if truncated { "..."}
      })
    ], [
      #text(fill: color, symbol)
      #if is_disabled {
        "Übersprungen"
      } else if has_failed {
        "Fehlgeschlagen"
      } else if has_errors {
        "Fehler"
      } else {
        "Erfolgreich"
      }
    ], [#testcase.time])
  }

  table(
    columns: 3,
    table.header(repeat: true,)[*Testname*][*Status*][*Zeit*],
    ..for testsuite in data.testsuites {
      (table.header(repeat: true, level: 2, table.cell(colspan: 3)[*#testsuite.name*]),)
      for testcase in testsuite.testsuite {
        entry(testcase)
      }
    },
  )
}


