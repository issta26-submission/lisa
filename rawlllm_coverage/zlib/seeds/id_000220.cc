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
//<ID> 220
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Includes inside the function to make this snippet self-contained.
    // (Preprocessor directives are allowed anywhere in a translation unit.)
    #include <zlib.h>
    #include <string>
    #include <vector>
    #include <cstring>

    // Sample input data
    const std::string input = 
        "The quick brown fox jumps over the lazy dog. "
        "This is a test string for zlib compression and decompression. "
        "Repeat: The quick brown fox jumps over the lazy dog. "
        "End of test.";

    int ret = 0;
    z_stream defstrm;
    z_stream infstrm;
    std::vector<unsigned char> comp;
    std::vector<unsigned char> decomp;

    // Initialize streams to zero
    std::memset(&defstrm, 0, sizeof(defstrm));
    std::memset(&infstrm, 0, sizeof(infstrm));

    // 1) Initialize deflate (compress) stream
    ret = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        // deflateInit failed
        return -1;
    }

    // Prepare input for deflate
    defstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstrm.avail_in = static_cast<uInt>(input.size());

    // Reserve an initial output buffer. compressBound gives an upper bound for compressed size.
    comp.resize(static_cast<size_t>(compressBound(static_cast<uLong>(input.size()))));
    defstrm.next_out = comp.data();
    defstrm.avail_out = static_cast<uInt>(comp.size());

    // 2) Perform deflate (may require multiple calls if output buffer fills)
    while (true) {
        ret = deflate(&defstrm, Z_FINISH); // finish compression
        if (ret == Z_STREAM_END) {
            // Finished successfully
            break;
        }
        if (ret == Z_OK) {
            // Output buffer was probably too small; expand and continue
            size_t used = comp.size() - defstrm.avail_out;
            size_t newSize = comp.size() * 2;
            if (newSize < used + 1) newSize = used + 1;
            comp.resize(newSize);
            defstrm.next_out = comp.data() + used;
            defstrm.avail_out = static_cast<uInt>(comp.size() - used);
            continue;
        }
        // Some other error
        deflateEnd(&defstrm);
        return -2;
    }

    // Compute final compressed size
    size_t compressedSize = comp.size() - defstrm.avail_out;

    // 3) Clean up deflate
    ret = deflateEnd(&defstrm);
    if (ret != Z_OK) {
        return -3;
    }

    // 4) Initialize inflate (decompress) stream
    ret = inflateInit(&infstrm);
    if (ret != Z_OK) {
        return -4;
    }

    // Prepare input for inflate using the compressed data
    infstrm.next_in = comp.data();
    infstrm.avail_in = static_cast<uInt>(compressedSize);

    // Prepare output buffer. Start with original size and grow if needed.
    decomp.resize(input.size());
    infstrm.next_out = decomp.data();
    infstrm.avail_out = static_cast<uInt>(decomp.size());

    // 5) Perform inflate (may require multiple calls if output buffer fills)
    while (true) {
        ret = inflate(&infstrm, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) {
            // Finished successfully
            break;
        }
        if (ret == Z_OK) {
            // Output buffer probably too small; expand and continue
            size_t used = decomp.size() - infstrm.avail_out;
            size_t newSize = decomp.size() * 2;
            if (newSize < used + 1) newSize = used + 1;
            decomp.resize(newSize);
            infstrm.next_out = decomp.data() + used;
            infstrm.avail_out = static_cast<uInt>(decomp.size() - used);
            continue;
        }
        // Some other error
        inflateEnd(&infstrm);
        return -5;
    }

    // Compute final decompressed size
    size_t decompressedSize = decomp.size() - infstrm.avail_out;

    // 6) Clean up inflate
    ret = inflateEnd(&infstrm);
    if (ret != Z_OK) {
        return -6;
    }

    // 7) Verify decompressed data matches original
    if (decompressedSize != input.size()) {
        return -7;
    }
    if (std::memcmp(decomp.data(), input.data(), input.size()) != 0) {
        return -8;
    }

    // 8) Use crc32 API to verify checksum matches
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(input.data()), static_cast<uInt>(input.size()));
    uLong crc_decomp = crc32(0L, Z_NULL, 0);
    crc_decomp = crc32(crc_decomp, decomp.data(), static_cast<uInt>(decompressedSize));
    if (crc_orig != crc_decomp) {
        return -9;
    }

    // All checks passed; return 66 as requested
    return 66;
}