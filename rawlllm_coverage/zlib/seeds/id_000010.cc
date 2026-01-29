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
//<ID> 10
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

// Returns 66 on success; other values indicate failure.
int test_zlib_api_sequence() {
    // Example input (repeated to make it reasonably compressible)
    const std::string input = std::string("The quick brown fox jumps over the lazy dog. ") + 
                              std::string("The quick brown fox jumps over the lazy dog. ") +
                              std::string("The quick brown fox jumps over the lazy dog. ") +
                              std::string("The quick brown fox jumps over the lazy dog. ");

    const uLong input_len = static_cast<uLong>(input.size());

    // Compute original checksums
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(input.data()), input_len);
    uLong adl_orig = adler32(0L, Z_NULL, 0);
    adl_orig = adler32(adl_orig, reinterpret_cast<const Bytef*>(input.data()), input_len);

    // ---- Streaming compression using deflateInit / deflate / deflateEnd ----
    z_stream dstrm;
    std::memset(&dstrm, 0, sizeof(dstrm));
    int ret = deflateInit(&dstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -1;

    dstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    dstrm.avail_in = static_cast<uInt>(input_len); // safe for small example

    // Ask for an upper bound for compressed data
    uLong bound = deflateBound(&dstrm, input_len);
    std::vector<Bytef> comp_buf(bound);
    dstrm.next_out = comp_buf.data();
    dstrm.avail_out = static_cast<uInt>(comp_buf.size());

    // Perform compression. Z_FINISH since we provide all input at once.
    ret = deflate(&dstrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&dstrm);
        return -2;
    }
    // compressed size
    const uLong comp_size = static_cast<uLong>(comp_buf.size() - dstrm.avail_out);
    deflateEnd(&dstrm);

    // ---- Streaming decompression using inflateInit / inflate / inflateEnd ----
    z_stream istrm;
    std::memset(&istrm, 0, sizeof(istrm));
    istrm.next_in = comp_buf.data();
    istrm.avail_in = static_cast<uInt>(comp_size);

    std::vector<Bytef> decomp_buf(input_len); // expected output size
    istrm.next_out = decomp_buf.data();
    istrm.avail_out = static_cast<uInt>(decomp_buf.size());

    ret = inflateInit(&istrm);
    if (ret != Z_OK) return -3;

    // Loop until stream end (handle cases where output buffer might be too small)
    do {
        ret = inflate(&istrm, Z_NO_FLUSH);
        if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            inflateEnd(&istrm);
            return -4;
        }
        // If output buffer becomes full but stream not finished, that would require resizing output buffer.
        // For this example we sized it to the original input size.
    } while (ret != Z_STREAM_END);

    const uLong decomp_size = istrm.total_out;
    inflateEnd(&istrm);

    if (decomp_size != input_len) return -5;
    if (std::memcmp(decomp_buf.data(), input.data(), input_len) != 0) return -6;

    // ---- Convenience one-shot compression/uncompression using compress2 / uncompress ----
    uLong cbound = compressBound(input_len);
    std::vector<Bytef> comp2_buf(cbound);
    uLong comp2_size = cbound;
    ret = compress2(comp2_buf.data(), &comp2_size,
                    reinterpret_cast<const Bytef*>(input.data()), input_len,
                    Z_BEST_COMPRESSION);
    if (ret != Z_OK) return -7;

    std::vector<Bytef> decomp2_buf(input_len);
    uLong decomp2_size = input_len;
    ret = uncompress(decomp2_buf.data(), &decomp2_size, comp2_buf.data(), comp2_size);
    if (ret != Z_OK) return -8;
    if (decomp2_size != input_len) return -9;
    if (std::memcmp(decomp2_buf.data(), input.data(), input_len) != 0) return -10;

    // Verify checksums match original
    uLong crc_decomp = crc32(0L, Z_NULL, 0);
    crc_decomp = crc32(crc_decomp, decomp2_buf.data(), decomp2_size);
    uLong adl_decomp = adler32(0L, Z_NULL, 0);
    adl_decomp = adler32(adl_decomp, decomp2_buf.data(), decomp2_size);
    if (crc_decomp != crc_orig) return -11;
    if (adl_decomp != adl_orig) return -12;

    // All tests passed
    return 66;
}