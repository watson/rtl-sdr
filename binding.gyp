{
  "targets": [
    {
      "target_name": "rtlsdr",
      "sources": [ "src/binding.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      "libraries": [
        "-lrtlsdr"
      ],
    }
  ]
}
