# Description

Quickly compute a JPEG's hash using only its image data (pixels), while disregarding any meta data. Useful for finding duplicates.

For hashes we use the MD5 algorithm.

# Compiling

The files are packaged as a Code::Blocks C++ project.

# Usage

```bash
jpeg-image-hash /path/to/file.jpg
```