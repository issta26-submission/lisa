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
//<ID> 129
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample input data
    const std::string input =
        "The quick brown fox jumps over the lazy dog. "
        "Zlib API sequence test. This string will be compressed and then "
        "decompressed to verify the sequence of zlib calls.";

    // -------------------------
    // 1) Compress using deflate
    // -------------------------
    z_stream defstream{};
    defstream.zalloc = Z_NULL;
    defstream.zfree  = Z_NULL;
    defstream.opaque = Z_NULL;

    defstream.next_in  = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(input.data()));
    defstream.avail_in = static_cast<uInt>(input.size()); // safe as input.size() is small here

    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return -1;
    }

    // allocate output buffer using compressBound to ensure it's large enough
    uLong bound = compressBound(static_cast<uLong>(input.size()));
    std::vector<unsigned char> compBuf(bound);

    defstream.next_out  = compBuf.data();
    defstream.avail_out = static_cast<uInt>(compBuf.size());

    // Perform compression. Using Z_FINISH because we provide all input at once.
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // cleanup
        deflateEnd(&defstream);
        return -2;
    }

    // compressed size
    size_t compSize = compBuf.size() - defstream.avail_out;

    ret = deflateEnd(&defstream);
    if (ret != Z_OK) {
        return -3;
    }

    // -------------------------
    // 2) Decompress using inflate
    // -------------------------
    z_stream infstream{};
    infstream.zalloc = Z_NULL;
    infstream.zfree  = Z_NULL;
    infstream.opaque = Z_NULL;

    infstream.next_in  = compBuf.data();
    infstream.avail_in = static_cast<uInt>(compSize);

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return -4;
    }

    // Allocate output buffer of the original size (we expect to recover exactly that)
    std::vector<unsigned char> decompBuf(input.size());
    infstream.next_out  = decompBuf.data();
    infstream.avail_out = static_cast<uInt>(decompBuf.size());

    // Perform decompression. Using Z_FINISH because we expect to provide all compressed data and receive all output.
    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return -5;
    }

    size_t decompSize = decompBuf.size() - infstream.avail_out;

    ret = inflateEnd(&infstream);
    if (ret != Z_OK) {
        return -6;
    }

    // -------------------------
    // 3) Verify correctness
    // -------------------------
    if (decompSize != input.size()) {
        return -7;
    }
    if (std::memcmp(decompBuf.data(), input.data(), input.size()) != 0) {
        return -8;
    }

    // All good
    return 66;
}