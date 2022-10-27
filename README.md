fast-zstd exposes native bindings to the zstd library.

Installation
===

`npm install fast-zstd`

API
===

```
Uint8Array zstd.compress(Uint8Array dataToCompress, int|CDict optionalCompressionLevelOrCDict)
Uint8Array zstd.decompress(Uint8Array dataToDecompress, DDict optionalDDict)
CDict zstd.createCDict(Uint8Array rawDictData);
DDict zstd.createDDict(Uint8Array rawDictData);
```

Note: if you used a CDict to compress, you must pass the DDict created from the
same raw dictionary when decompressing.
