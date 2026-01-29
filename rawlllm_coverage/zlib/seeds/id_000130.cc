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
//<ID> 130
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers locally so this function is self-contained when pasted alone.
    // Some build systems expect includes at file scope; if so, move these to the top of your file.
    #include <zlib.h>
    #include <cstring>
    #include <cstdlib>

    // Example input data to compress and decompress
    const char* input = "The quick brown fox jumps over the lazy dog. "
                        "The quick brown fox jumps over the lazy dog. "
                        "Repeat to ensure non-trivial compression.";
    uLong input_len = static_cast<uLong>(std::strlen(input));

    // Calculate maximum compressed size and allocate buffer
    uLong max_compressed = compressBound(input_len);
    Bytef* comp_buf = static_cast<Bytef*>(std::malloc(max_compressed));
    if (!comp_buf) return 1;

    // Initialize deflate (compression) stream
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        std::free(comp_buf);
        return 2;
    }

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    defstream.avail_in = input_len;
    defstream.next_out = comp_buf;
    defstream.avail_out = max_compressed;

    // Perform compression with Z_FINISH since we provide full input
    ret = deflate(&defstream, Z_FINISH);
    while (ret == Z_OK) {
        // If buffer wasn't large enough (shouldn't happen thanks to compressBound),
        // we'd need to reallocate and continue. Handle just in case.
        uLong old_max = max_compressed;
        max_compressed *= 2;
        Bytef* new_buf = static_cast<Bytef*>(std::realloc(comp_buf, max_compressed));
        if (!new_buf) {
            deflateEnd(&defstream);
            std::free(comp_buf);
            return 3;
        }
        comp_buf = new_buf;
        defstream.next_out = comp_buf + (max_compressed - defstream.avail_out - (max_compressed - old_max));
        defstream.avail_out = max_compressed - (old_max - (defstream.avail_out));
        ret = deflate(&defstream, Z_FINISH);
    }

    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        std::free(comp_buf);
        return 4;
    }

    uLong compressed_size = max_compressed - defstream.avail_out;
    deflateEnd(&defstream);

    // Compute checksums on original data
    uLong adler_orig = adler32(0L, Z_NULL, 0);
    adler_orig = adler32(adler_orig, reinterpret_cast<const Bytef*>(input), input_len);
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(input), input_len);

    // Allocate buffer for decompression (we know the original size)
    Bytef* decomp_buf = static_cast<Bytef*>(std::malloc(input_len));
    if (!decomp_buf) {
        std::free(comp_buf);
        return 5;
    }

    // Initialize inflate (decompression) stream
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        std::free(comp_buf);
        std::free(decomp_buf);
        return 6;
    }

    infstream.next_in = comp_buf;
    infstream.avail_in = compressed_size;
    infstream.next_out = decomp_buf;
    infstream.avail_out = input_len;

    // Perform inflation
    ret = inflate(&infstream, Z_FINISH);
    while (ret == Z_OK) {
        // Not expected because output buffer equals original size, but handle gracefully.
        // If needed, reallocate decomp_buf larger and continue.
        std::free(comp_buf);
        inflateEnd(&infstream);
        std::free(decomp_buf);
        return 7;
    }

    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        std::free(comp_buf);
        std::free(decomp_buf);
        return 8;
    }

    uLong decomp_size = input_len - infstream.avail_out;
    inflateEnd(&infstream);

    // Verify decompressed size and content match original
    if (decomp_size != input_len) {
        std::free(comp_buf);
        std::free(decomp_buf);
        return 9;
    }
    if (std::memcmp(decomp_buf, input, input_len) != 0) {
        std::free(comp_buf);
        std::free(decomp_buf);
        return 10;
    }

    // Recompute checksum on decompressed data and compare
    uLong adler_decomp = adler32(0L, Z_NULL, 0);
    adler_decomp = adler32(adler_decomp, decomp_buf, decomp_size);
    if (adler_decomp != adler_orig) {
        std::free(comp_buf);
        std::free(decomp_buf);
        return 11;
    }
    uLong crc_decomp = crc32(0L, Z_NULL, 0);
    crc_decomp = crc32(crc_decomp, decomp_buf, decomp_size);
    if (crc_decomp != crc_orig) {
        std::free(comp_buf);
        std::free(decomp_buf);
        return 12;
    }

    // Clean up
    std::free(comp_buf);
    std::free(decomp_buf);

    // If we reached here, all zlib API sequences succeeded and verification passed.
    return 66;
}