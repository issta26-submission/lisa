#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 195
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here so this function is self-contained.
    // (Including inside a function is valid C/C++ preprocessor usage.)
    #include <zlib.h>
    #include <vector>
    #include <string>
    #include <cstring>

    // Prepare some test data (repeat a phrase to make it compressible).
    std::string original;
    for (int i = 0; i < 20; ++i) original += "The quick brown fox jumps over the lazy dog. ";

    size_t orig_len = original.size();
    if (orig_len == 0) return 1;

    // Compute Adler-32 and CRC32 of the original data.
    uLong adler_orig = adler32(0L, Z_NULL, 0);
    adler_orig = adler32(adler_orig, reinterpret_cast<const Bytef*>(original.data()), static_cast<uInt>(orig_len));
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(original.data()), static_cast<uInt>(orig_len));

    // --- High-level compress/uncompress using compress2/uncompress ---
    uLong sourceLen = static_cast<uLong>(orig_len);
    uLongf compBound = compressBound(sourceLen);
    std::vector<Bytef> compBuf(static_cast<size_t>(compBound));

    int zres = compress2(compBuf.data(), &compBound,
                         reinterpret_cast<const Bytef*>(original.data()), sourceLen,
                         Z_BEST_COMPRESSION);
    if (zres != Z_OK) return 2; // compress2 failed

    // Now uncompress
    std::vector<Bytef> uncompressedBuf(orig_len);
    uLongf uncompressedSize = static_cast<uLongf>(orig_len);
    zres = uncompress(uncompressedBuf.data(), &uncompressedSize, compBuf.data(), compBound);
    if (zres != Z_OK) return 3; // uncompress failed
    if (uncompressedSize != sourceLen) return 4; // size mismatch
    if (std::memcmp(uncompressedBuf.data(), original.data(), orig_len) != 0) return 5; // content mismatch

    // Verify checksums match
    uLong adler_after_high = adler32(0L, Z_NULL, 0);
    adler_after_high = adler32(adler_after_high, uncompressedBuf.data(), static_cast<uInt>(uncompressedSize));
    if (adler_after_high != adler_orig) return 6;
    uLong crc_after_high = crc32(0L, Z_NULL, 0);
    crc_after_high = crc32(crc_after_high, uncompressedBuf.data(), static_cast<uInt>(uncompressedSize));
    if (crc_after_high != crc_orig) return 7;

    // --- Low-level deflate/inflate using z_stream ---
    z_stream defstrm;
    std::memset(&defstrm, 0, sizeof(defstrm));
    defstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original.data()));
    defstrm.avail_in = static_cast<uInt>(orig_len);

    // Use compressBound to size the deflate output buffer
    uLongf defBufSize = compressBound(sourceLen);
    std::vector<Bytef> deflatedBuf(static_cast<size_t>(defBufSize));
    defstrm.next_out = deflatedBuf.data();
    defstrm.avail_out = static_cast<uInt>(defBufSize);

    zres = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
    if (zres != Z_OK) return 8;
    zres = deflate(&defstrm, Z_FINISH);
    if (zres != Z_STREAM_END) { deflateEnd(&defstrm); return 9; }
    size_t deflatedSize = defBufSize - defstrm.avail_out;
    deflateEnd(&defstrm);

    // Inflate back
    z_stream infstrm;
    std::memset(&infstrm, 0, sizeof(infstrm));
    infstrm.next_in = deflatedBuf.data();
    infstrm.avail_in = static_cast<uInt>(deflatedSize);

    std::vector<Bytef> inflatedBuf(orig_len);
    infstrm.next_out = inflatedBuf.data();
    infstrm.avail_out = static_cast<uInt>(orig_len);

    zres = inflateInit(&infstrm);
    if (zres != Z_OK) return 10;
    zres = inflate(&infstrm, Z_FINISH);
    if (zres != Z_STREAM_END) { inflateEnd(&infstrm); return 11; }
    size_t inflatedSize = orig_len - infstrm.avail_out;
    inflateEnd(&infstrm);

    if (inflatedSize != orig_len) return 12;
    if (std::memcmp(inflatedBuf.data(), original.data(), orig_len) != 0) return 13;

    // Verify checksums on the inflated data match originals
    uLong adler_after_low = adler32(0L, Z_NULL, 0);
    adler_after_low = adler32(adler_after_low, inflatedBuf.data(), static_cast<uInt>(inflatedSize));
    if (adler_after_low != adler_orig) return 14;
    uLong crc_after_low = crc32(0L, Z_NULL, 0);
    crc_after_low = crc32(crc_after_low, inflatedBuf.data(), static_cast<uInt>(inflatedSize));
    if (crc_after_low != crc_orig) return 15;

    // All checks passed, return 66 per specification.
    return 66;
}