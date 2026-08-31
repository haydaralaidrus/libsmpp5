# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- GNU LGPL license.
- Supports `install()` with standard paths, CMake package export, pkg-config support,
  & versioned shared libraries.

### Changed
- Public headers moved from `src/` to `include/libsmpp5/`.

## [0.0.1] - 2026-08-31

### Added
- Initialize project.
- `pdu.h`/`pdu.c`: Encoding/decoding PDUs.
- `smpp.h`/`smpp.c`: SMPP functionality.

[unreleased]: https://github.com/haydaralaidrus/libsmpp5/compare/v0.0.1...HEAD
[0.0.1]: https://github.com/haydaralaidrus/libsmpp5/releases/tag/v0.0.1
