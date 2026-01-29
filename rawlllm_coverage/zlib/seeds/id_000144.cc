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
//<ID> 144
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

// Returns 66 on success; other negative values indicate different failure points.
int test_zlib_api_sequence() {
    // Original input to compress/decompress
    const std::string input =
        "The quick brown fox jumps over the lazy dog. "
        "This is a test string to exercise zlib APIs. "
        "Repeat to ensure enough data to exercise streaming paths. "
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog.";

    const uLong srcLen = static_cast<uLong>(input.size());

    // 1) Use compressBound + compress2 (simple one-shot API)
    uLong bound = compressBound(srcLen);
    std::vector<Bytef> comp1(bound);
    uLongf comp1Len = bound;
    int ret = compress2(comp1.data(), &comp1Len,
                        reinterpret_cast<const Bytef*>(input.data()), srcLen,
                        Z_BEST_COMPRESSION);
    if (ret != Z_OK) return -1; // compress2 failed

    // 2) Use uncompress to get original back
    std::vector<Bytef> decomp1(srcLen);
    uLongf decomp1Len = srcLen;
    ret = uncompress(decomp1.data(), &decomp1Len, comp1.data(), comp1Len);
    if (ret != Z_OK) return -2; // uncompress failed
    if (decomp1Len != srcLen) return -3; // unexpected decompressed length
    if (std::memcmp(decomp1.data(), input.data(), srcLen) != 0) return -4; // mismatch

    // 3) Use z_stream with deflateInit / deflate / deflateEnd (streaming compression)
    z_stream defstrm;
    std::memset(&defstrm, 0, sizeof(defstrm));
    ret = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -5;

    // Provide entire input as a single chunk and output buffer sized by compressBound
    std::vector<Bytef> comp2(bound);
    defstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstrm.avail_in = static_cast<uInt>(srcLen);
    defstrm.next_out = comp2.data();
    defstrm.avail_out = static_cast<uInt>(comp2.size());

    ret = deflate(&defstrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstrm);
        return -6; // streaming deflate did not finish as expected
    }
    uLongf comp2Len = static_cast<uLongf>(comp2.size()) - defstrm.avail_out;
    deflateEnd(&defstrm);

    // 4) Use z_stream with inflateInit / inflate / inflateEnd (streaming decompression)
    z_stream infstrm;
    std::memset(&infstrm, 0, sizeof(infstrm));
    ret = inflateInit(&infstrm);
    if (ret != Z_OK) return -7;

    std::vector<Bytef> decomp2(srcLen);
    infstrm.next_in = comp2.data();
    infstrm.avail_in = static_cast<uInt>(comp2Len);
    infstrm.next_out = decomp2.data();
    infstrm.avail_out = static_cast<uInt>(decomp2.size());

    ret = inflate(&infstrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstrm);
        return -8; // streaming inflate did not finish as expected
    }
    uLongf decomp2Len = static_cast<uLongf>(decomp2.size()) - infstrm.avail_out;
    inflateEnd(&infstrm);

    if (decomp2Len != srcLen) return -9; // unexpected length after inflate
    if (std::memcmp(decomp2.data(), input.data(), srcLen) != 0) return -10; // mismatch

    // 5) Use checksum helpers (crc32 and adler32)
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input.data()), srcLen);
    // compute adler32 too
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(input.data()), srcLen);

    // Sanity check: CRC and Adler should be non-zero for non-empty input
    if (crc == 0 || adl == 0) return -11;

    // If we reach here, all tested zlib APIs worked as expected.
    return 66;
}