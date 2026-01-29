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
//<ID> 3
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates a correct usage sequence of the zlib API:
    // - deflateInit / deflate / deflateEnd to compress data
    // - inflateInit / inflate / inflateEnd to decompress data
    // - crc32 to verify integrity
    // - compress2 / uncompress as an alternate convenience API check
    //
    // On success the function returns 66. On any error it returns a non-66 value.

    // NOTE: This function assumes the including translation unit includes <zlib.h>
    // and the standard headers required for the used types (e.g., <vector>, <string>, <cstring>).

    // Prepare input data
    const char* sample = "The quick brown fox jumps over the lazy dog. ";
    std::string input;
    // Repeat the sample several times to make input non-trivial
    for (int i = 0; i < 16; ++i) input += sample;
    const size_t input_size = input.size();

    // --- 1) Compress using deflate streams ---
    z_stream defstrm;
    std::memset(&defstrm, 0, sizeof(defstrm));
    int ret = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 1;

    // Allocate output buffer using compressBound to ensure it's large enough
    uLong bound = compressBound(static_cast<uLong>(input_size));
    std::vector<unsigned char> compBuf(bound);
    defstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstrm.avail_in = static_cast<uInt>(input_size);
    defstrm.next_out = compBuf.data();
    defstrm.avail_out = static_cast<uInt>(compBuf.size());

    // Perform compression with Z_FINISH; because we used compressBound, a single call should finish.
    ret = deflate(&defstrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // If we didn't finish, try to handle by expanding buffer (robust fallback)
        while (ret == Z_OK) {
            // enlarge buffer by chunks
            size_t oldSize = compBuf.size();
            size_t used = oldSize - defstrm.avail_out;
            compBuf.resize(oldSize + 16384);
            defstrm.next_out = compBuf.data() + used;
            defstrm.avail_out = static_cast<uInt>(compBuf.size() - used);
            ret = deflate(&defstrm, Z_FINISH);
        }
        if (ret != Z_STREAM_END) {
            deflateEnd(&defstrm);
            return 2;
        }
    }
    // compressed size
    size_t compressed_size = compBuf.size() - defstrm.avail_out;

    // Clean up deflate state
    if (deflateEnd(&defstrm) != Z_OK) return 3;

    // --- 2) Decompress using inflate streams ---
    z_stream infstrm;
    std::memset(&infstrm, 0, sizeof(infstrm));
    ret = inflateInit(&infstrm);
    if (ret != Z_OK) return 4;

    std::vector<unsigned char> decompBuf(input_size); // we expect original size
    infstrm.next_in = compBuf.data();
    infstrm.avail_in = static_cast<uInt>(compressed_size);
    infstrm.next_out = decompBuf.data();
    infstrm.avail_out = static_cast<uInt>(decompBuf.size());

    ret = inflate(&infstrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // If not finished, handle expansion (should not happen if we allocated exact size)
        while (ret == Z_OK) {
            size_t oldSize = decompBuf.size();
            size_t used = oldSize - infstrm.avail_out;
            decompBuf.resize(oldSize + 16384);
            infstrm.next_out = decompBuf.data() + used;
            infstrm.avail_out = static_cast<uInt>(decompBuf.size() - used);
            ret = inflate(&infstrm, Z_FINISH);
        }
        if (ret != Z_STREAM_END) {
            inflateEnd(&infstrm);
            return 5;
        }
    }

    size_t decompressed_size = decompBuf.size() - infstrm.avail_out;
    if (inflateEnd(&infstrm) != Z_OK) return 6;

    // Verify sizes and content match original
    if (decompressed_size != input_size) return 7;
    if (std::memcmp(decompBuf.data(), input.data(), input_size) != 0) return 8;

    // --- 3) Verify CRC using crc32 ---
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(input.data()), static_cast<uInt>(input_size));
    uLong crc_decomp = crc32(0L, Z_NULL, 0);
    crc_decomp = crc32(crc_decomp, decompBuf.data(), static_cast<uInt>(decompressed_size));
    if (crc_orig != crc_decomp) return 9;

    // --- 4) Quick check using convenience APIs: compress2 / uncompress ---
    uLong bound2 = compressBound(static_cast<uLong>(input_size));
    std::vector<unsigned char> comp2(bound2);
    uLongf comp2_size = bound2;
    ret = compress2(comp2.data(), &comp2_size,
                    reinterpret_cast<const Bytef*>(input.data()),
                    static_cast<uLong>(input_size),
                    Z_BEST_SPEED);
    if (ret != Z_OK) return 10;

    std::vector<unsigned char> uncomp2(input_size);
    uLongf uncomp2_size = static_cast<uLongf>(uncomp2.size());
    ret = uncompress(uncomp2.data(), &uncomp2_size, comp2.data(), comp2_size);
    if (ret != Z_OK) return 11;
    if (uncomp2_size != input_size) return 12;
    if (std::memcmp(uncomp2.data(), input.data(), input_size) != 0) return 13;

    // All checks passed
    return 66;
}