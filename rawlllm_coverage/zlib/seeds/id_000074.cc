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
//<ID> 74
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Prepare sample data (repeated pattern to make compression effective)
    std::string pattern = "The quick brown fox jumps over the lazy dog. ";
    std::string data;
    while (data.size() < 2048) data += pattern;
    data.resize(2048);
    uLong srcLen = static_cast<uLong>(data.size());

    // Compute checksums (CRC32 and Adler-32) of the original data
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(data.data()), srcLen);
    uLong adler_orig = adler32(0L, Z_NULL, 0);
    adler_orig = adler32(adler_orig, reinterpret_cast<const Bytef*>(data.data()), srcLen);

    // 1) High-level compress/uncompress using compress2/uncompress
    uLong compBound = compressBound(srcLen);
    std::vector<Bytef> compBuf(compBound);
    uLong compLen = compBound;
    int ret = compress2(compBuf.data(), &compLen,
                        reinterpret_cast<const Bytef*>(data.data()), srcLen,
                        Z_BEST_COMPRESSION);
    if (ret != Z_OK) return 1;
    compBuf.resize(compLen);

    std::vector<Bytef> uncmpBuf(srcLen);
    uLong uncmpLen = srcLen;
    ret = uncompress(uncmpBuf.data(), &uncmpLen, compBuf.data(), compLen);
    if (ret != Z_OK) return 2;
    if (uncmpLen != srcLen) return 3;
    if (std::memcmp(uncmpBuf.data(), data.data(), srcLen) != 0) return 4;

    // 2) Stream-based deflate (z_stream / deflate) then inflate back
    z_stream defStrm{};
    ret = deflateInit(&defStrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 5;

    // deflateBound can be used to get a safe upper bound for output buffer
    uLong defBound = deflateBound(&defStrm, srcLen);
    std::vector<Bytef> deflated(defBound);

    // Set input and output for deflate
    defStrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(data.data()));
    defStrm.avail_in = static_cast<uInt>(srcLen); // safe for our sample size
    defStrm.next_out = deflated.data();
    defStrm.avail_out = static_cast<uInt>(defBound);

    ret = deflate(&defStrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defStrm);
        return 6;
    }
    uLong deflatedSize = defBound - defStrm.avail_out;
    deflateEnd(&defStrm);
    deflated.resize(deflatedSize);

    // Inflate back
    z_stream infStrm{};
    ret = inflateInit(&infStrm);
    if (ret != Z_OK) return 7;

    std::vector<Bytef> inflated(srcLen);
    infStrm.next_in = deflated.data();
    infStrm.avail_in = static_cast<uInt>(deflatedSize);
    infStrm.next_out = inflated.data();
    infStrm.avail_out = static_cast<uInt>(srcLen);

    ret = inflate(&infStrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infStrm);
        return 8;
    }
    uLong inflatedSize = srcLen - infStrm.avail_out;
    inflateEnd(&infStrm);

    if (inflatedSize != srcLen) return 9;
    if (std::memcmp(inflated.data(), data.data(), srcLen) != 0) return 10;

    // 3) Verify checksums match original for the inflated data
    uLong crc_after = crc32(0L, Z_NULL, 0);
    crc_after = crc32(crc_after, inflated.data(), inflatedSize);
    uLong adler_after = adler32(0L, Z_NULL, 0);
    adler_after = adler32(adler_after, inflated.data(), inflatedSize);

    if (crc_after != crc_orig) return 11;
    if (adler_after != adler_orig) return 12;

    // If we arrive here, all tested zlib APIs behaved correctly.
    return 66;
}