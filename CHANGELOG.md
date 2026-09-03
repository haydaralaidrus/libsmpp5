# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [1.1.0] - 2026-09-03

### Added
- Every operation backed by a shared struct now gets its own typedef & function
  names. All static inline & no new codec logic.

### Fixed
- Fix library version mismatch in the root cmake `./CMakeList.txt`.

## [1.0.0] - 2026-09-01

### Added
- Error to string functions: `pdu_strerror`, `smpp_strerror`, `smpp_tlv_strerror`, & `smpp_esme_strerror`
- Doxygen API docs.
- GNU LGPL license.
- Supports `install()` with standard paths, CMake package export, pkg-config support,
  & versioned shared libraries.
- The remaining SMPP operations.

### Changed
- Public headers moved from `src/` to `include/libsmpp5/`.
- `smpp.c` & `tests/smpp.c`: split by operation family.

## [0.0.1] - 2026-08-31

### Added
- Initialize project.
- `pdu.h`/`pdu.c`: Encoding/decoding PDUs.
- `smpp.h`/`smpp.c`: SMPP functionality.

[unreleased]: https://github.com/haydaralaidrus/libsmpp5/compare/v1.1.0...HEAD
[1.1.0]: https://github.com/haydaralaidrus/libsmpp5/compare/v1.0.0...v1.1.0
[1.0.0]: https://github.com/haydaralaidrus/libsmpp5/compare/v0.0.1...v1.0.0
[0.0.1]: https://github.com/haydaralaidrus/libsmpp5/releases/tag/v0.0.1
