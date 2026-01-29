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
//<ID> 182
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

// Returns 66 on success. Any non-66 return indicates failure.
int test_zlib_api_sequence() {
    // Prepare input data (make it sufficiently large to exercise streaming)
    std::string original = "The quick brown fox jumps over the lazy dog. ";
    // repeat to enlarge
    for (int i = 0; i < 20; ++i) original += original;

    // --- 1) High-level API: compress() and uncompress() ---
    uLong srcLen = static_cast<uLong>(original.size());
    uLong bound = compressBound(srcLen);
    std::vector<Bytef> comp1(bound);
    uLong comp1Len = bound;

    int zres = compress(comp1.data(), &comp1Len,
                        reinterpret_cast<const Bytef*>(original.data()), srcLen);
    if (zres != Z_OK) return -2; // compress failed

    // Decompress with uncompress()
    std::vector<Bytef> un1(srcLen);
    uLong un1Len = srcLen;
    zres = uncompress(un1.data(), &un1Len, comp1.data(), comp1Len);
    if (zres != Z_OK) return -3; // uncompress failed
    if (un1Len != srcLen) return -4;
    if (std::memcmp(un1.data(), original.data(), srcLen) != 0) return -5;

    // Verify CRC using crc32
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(original.data()), srcLen);
    uLong crc_un1 = crc32(0L, Z_NULL, 0);
    crc_un1 = crc32(crc_un1, un1.data(), un1Len);
    if (crc_orig != crc_un1) return -6;

    // --- 2) Streaming API: deflate() and inflate() ---
    // For deflate we need a non-const input buffer pointer
    std::vector<Bytef> inBuf(original.begin(), original.end());

    // Deflate (compress) stream
    z_stream dstrm;
    std::memset(&dstrm, 0, sizeof(dstrm));
    if (deflateInit(&dstrm, Z_DEFAULT_COMPRESSION) != Z_OK) return -7;

    dstrm.next_in = inBuf.data();
    dstrm.avail_in = static_cast<uInt>(inBuf.size());

    std::vector<Bytef> comp2;
    int ret;
    do {
        Bytef outbuf[256];
        dstrm.next_out = outbuf;
        dstrm.avail_out = sizeof(outbuf);

        // Use Z_FINISH because we're providing all input at once
        ret = deflate(&dstrm, Z_FINISH);
        if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
            deflateEnd(&dstrm);
            return -8;
        }
        size_t have = sizeof(outbuf) - dstrm.avail_out;
        comp2.insert(comp2.end(), outbuf, outbuf + have);
    } while (ret != Z_STREAM_END);
    deflateEnd(&dstrm);

    // Inflate (decompress) stream
    z_stream istrm;
    std::memset(&istrm, 0, sizeof(istrm));
    if (inflateInit(&istrm) != Z_OK) return -9;

    istrm.next_in = comp2.data();
    istrm.avail_in = static_cast<uInt>(comp2.size());

    std::vector<Bytef> un2;
    do {
        Bytef outbuf[256];
        istrm.next_out = outbuf;
        istrm.avail_out = sizeof(outbuf);

        ret = inflate(&istrm, Z_NO_FLUSH);
        if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
            inflateEnd(&istrm);
            return -10;
        }
        size_t have = sizeof(outbuf) - istrm.avail_out;
        un2.insert(un2.end(), outbuf, outbuf + have);
    } while (ret != Z_STREAM_END);
    inflateEnd(&istrm);

    // Validate streaming round-trip
    if (un2.size() != inBuf.size()) return -11;
    if (std::memcmp(un2.data(), inBuf.data(), inBuf.size()) != 0) return -12;

    // CRC check on streaming result
    uLong crc_un2 = crc32(0L, Z_NULL, 0);
    crc_un2 = crc32(crc_un2, un2.data(), static_cast<uInt>(un2.size()));
    if (crc_un2 != crc_orig) return -13;

    // All checks passed
    return 66;
}