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
//<ID> 219
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample input to compress/decompress
    const std::string input = "The quick brown fox jumps over the lazy dog. "
                              "This is a test of zlib's compress/uncompress and "
                              "deflate/inflate streaming APIs.";

    // --- 1) Simple one-shot compress/uncompress using compress2/uncompress ---

    uLong src_len = static_cast<uLong>(input.size());
    // compute an upper bound for compressed data
    uLong comp_bound = compressBound(src_len);
    std::vector<unsigned char> compressed(comp_bound);

    // compress with best compression level
    int ret = compress2(compressed.data(), &comp_bound,
                        reinterpret_cast<const Bytef*>(input.data()), src_len,
                        Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        return -1; // compress failed
    }
    compressed.resize(comp_bound);

    // prepare output buffer for uncompress (we know expected size)
    std::vector<unsigned char> decompressed(src_len);
    uLong decompressed_len = src_len;
    ret = uncompress(decompressed.data(), &decompressed_len,
                     compressed.data(), compressed.size());
    if (ret != Z_OK) {
        return -2; // uncompress failed
    }
    if (decompressed_len != src_len ||
        std::memcmp(decompressed.data(), input.data(), src_len) != 0) {
        return -3; // data mismatch
    }

    // --- 2) Streaming compress (deflate) and inflate using z_stream ---

    // deflate (streaming)
    z_stream def_stream;
    std::memset(&def_stream, 0, sizeof(def_stream));
    def_stream.zalloc = Z_NULL;
    def_stream.zfree = Z_NULL;
    def_stream.opaque = Z_NULL;

    ret = deflateInit(&def_stream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return -4; // deflateInit failed
    }

    // Provide all input at once (small input), allocate enough output
    def_stream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    def_stream.avail_in = static_cast<uInt>(input.size());

    uLong comp2_bound = compressBound(src_len);
    std::vector<unsigned char> comp2(comp2_bound);
    def_stream.next_out = comp2.data();
    def_stream.avail_out = static_cast<uInt>(comp2.size());

    ret = deflate(&def_stream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&def_stream);
        return -5; // deflate did not finish as expected
    }
    std::size_t comp2_size = comp2.size() - def_stream.avail_out;
    deflateEnd(&def_stream);
    comp2.resize(comp2_size);

    // inflate (streaming)
    z_stream inf_stream;
    std::memset(&inf_stream, 0, sizeof(inf_stream));
    inf_stream.zalloc = Z_NULL;
    inf_stream.zfree = Z_NULL;
    inf_stream.opaque = Z_NULL;

    ret = inflateInit(&inf_stream);
    if (ret != Z_OK) {
        return -6; // inflateInit failed
    }

    inf_stream.next_in = comp2.data();
    inf_stream.avail_in = static_cast<uInt>(comp2.size());

    std::vector<unsigned char> decomp2(src_len);
    inf_stream.next_out = decomp2.data();
    inf_stream.avail_out = static_cast<uInt>(decomp2.size());

    ret = inflate(&inf_stream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&inf_stream);
        return -7; // inflate did not finish as expected
    }
    std::size_t decomp2_size = decomp2.size() - inf_stream.avail_out;
    inflateEnd(&inf_stream);

    if (decomp2_size != src_len ||
        std::memcmp(decomp2.data(), input.data(), src_len) != 0) {
        return -8; // streamed roundtrip mismatch
    }

    // --- 3) Compute checksums (crc32 and adler32) to demonstrate additional APIs ---
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input.data()), src_len);

    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(input.data()), src_len);

    // Use the checksums in a trivial way to avoid unused-variable warnings
    if (crc == 0 && adl == 0) {
        // improbable for a non-empty input, but we won't treat it as failure
        // keep the variables referenced
    }

    // All checks passed
    return 66;
}