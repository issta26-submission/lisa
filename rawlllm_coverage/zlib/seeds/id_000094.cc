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
//<ID> 94
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers local to this function to keep the snippet self-contained.
    // (Including headers inside a function is permitted by the preprocessor, though not commonly used.)
    #include <zlib.h>
    #include <vector>
    #include <string>
    #include <cstring>

    // Prepare an input payload (make it somewhat sizable and repetitive so compression is effective).
    std::string original;
    for (int i = 0; i < 100; ++i) {
        original += "The quick brown fox jumps over the lazy dog. ";
    }
    uLong orig_len = static_cast<uLong>(original.size());

    // 1) Compress using the deflate API (deflateInit, deflate, deflateEnd).
    z_stream deflate_stream;
    std::memset(&deflate_stream, 0, sizeof(deflate_stream));
    int zret = deflateInit(&deflate_stream, Z_BEST_COMPRESSION);
    if (zret != Z_OK) {
        deflateEnd(&deflate_stream);
        return -1;
    }

    // Allocate output buffer using compressBound to guarantee sufficient space.
    uLong bound = compressBound(orig_len);
    std::vector<unsigned char> compressed(bound);

    deflate_stream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original.data()));
    deflate_stream.avail_in = orig_len;
    deflate_stream.next_out = compressed.data();
    deflate_stream.avail_out = static_cast<uInt>(compressed.size());

    zret = deflate(&deflate_stream, Z_FINISH);
    if (zret != Z_STREAM_END) {
        // cleanup
        deflateEnd(&deflate_stream);
        return -2;
    }
    // Compressed size:
    uLong compressed_size = static_cast<uLong>(compressed.size() - deflate_stream.avail_out);

    // finish and free deflate state
    zret = deflateEnd(&deflate_stream);
    if (zret != Z_OK) {
        return -3;
    }

    // 2) Decompress using the inflate API (inflateInit, inflate, inflateEnd).
    z_stream inflate_stream;
    std::memset(&inflate_stream, 0, sizeof(inflate_stream));
    zret = inflateInit(&inflate_stream);
    if (zret != Z_OK) {
        inflateEnd(&inflate_stream);
        return -4;
    }

    // Prepare destination buffer sized to original length (we know it).
    std::vector<unsigned char> decompressed(orig_len + 1); // +1 for safety / null termination if needed

    inflate_stream.next_in = compressed.data();
    inflate_stream.avail_in = static_cast<uInt>(compressed_size);
    inflate_stream.next_out = decompressed.data();
    inflate_stream.avail_out = static_cast<uInt>(decompressed.size());

    zret = inflate(&inflate_stream, Z_FINISH);
    if (zret != Z_STREAM_END) {
        inflateEnd(&inflate_stream);
        return -5;
    }
    uLong decompressed_size = static_cast<uLong>(decompressed.size() - inflate_stream.avail_out);

    zret = inflateEnd(&inflate_stream);
    if (zret != Z_OK) {
        return -6;
    }

    // 3) Verify decompressed data matches the original.
    if (decompressed_size != orig_len) {
        return -7;
    }
    if (std::memcmp(decompressed.data(), original.data(), orig_len) != 0) {
        return -8;
    }

    // If we reached here, compression -> decompression roundtrip succeeded.
    return 66;
}