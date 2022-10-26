let fs = require('fs');
let zstd = require('./build/Release/zstd');

let compressed = zstd.compress(Buffer.from('the quick brown fox', 'utf8'));
console.log(compressed.length);
console.log(compressed);
let decompressed = Buffer.from(zstd.decompress(compressed), 'utf8').toString();
console.log(decompressed);

