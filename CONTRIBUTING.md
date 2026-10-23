# Contributing

Contributions are GPL-3.0-only and require a Developer Certificate of Origin
`Signed-off-by` line. Do not submit Sony SDK material, firmware, keys or code
without documented redistribution rights. Dependency changes must pin an
immutable commit and hash and preserve corresponding source plus license texts.

Before changes, read [runtime ownership](docs/RUNTIME.md). Follow
[validation/release instructions](docs/VALIDATION.md); run `git diff --check`,
compile the example for API changes, and use sibling app regressions for timing
or buffer changes. Hardware fixes require physical evidence; mark unperformed
checks explicitly. Documentation updates must preserve historical measurements
and the difference between app and standalone FFmpeg prefixes. Do not bump the
2.0.0 ABI for a documentation-only refresh.
