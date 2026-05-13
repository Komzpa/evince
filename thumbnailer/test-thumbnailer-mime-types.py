#!/usr/bin/env python3

import pathlib
import sys


thumbnailer = pathlib.Path(sys.argv[1])
expect_pdf = sys.argv[2] == "true"

mime_line = next(
    line for line in thumbnailer.read_text(encoding="utf-8").splitlines()
    if line.startswith("MimeType=")
)
mime_types = set(filter(None, mime_line.removeprefix("MimeType=").split(";")))

for mime_type in {
    "application/postscript",
    "application/x-bzpostscript",
    "application/x-gzpostscript",
    "application/x-ext-eps",
    "application/x-ext-ps",
    "image/x-bzeps",
    "image/x-eps",
    "image/x-gzeps",
    "application/illustrator",
}:
    assert mime_type not in mime_types, mime_type

if expect_pdf:
    for mime_type in {
        "application/pdf",
        "application/x-bzpdf",
        "application/x-gzpdf",
    }:
        assert mime_type in mime_types, mime_type
