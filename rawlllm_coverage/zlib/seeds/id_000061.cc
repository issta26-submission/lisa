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
//<ID> 61
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates a correct sequence of zlib APIs:
    //  - initialize a deflate (compression) stream
    //  - deflate (streaming) the input to a compressed buffer
    //  - finish and cleanup the deflate stream
    //  - initialize an inflate (decompression) stream
    //  - inflate (streaming) the compressed buffer back to original
    //  - verify the decompressed output equals the original input
    // On success the function returns 66.
    //
    // Note: This function includes the required headers here so it is self-contained.
    #include <zlib.h>
    #include <vector>
    #include <string>
    #include <cstring>

    const std::string original =
        "The quick brown fox jumps over the lazy dog. "
        "This is a test string to exercise zlib's deflate/inflate APIs. "
        "Repeating to make it a bit larger for streaming behavior. "
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog.";

    // Chunk size for streaming
    const uInt CHUNK = 128;

    // --- Compression (deflate) ---
    z_stream c_stream;
    std::memset(&c_stream, 0, sizeof(c_stream));
    int ret = deflateInit(&c_stream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return 1; // deflateInit failed
    }

    std::vector<unsigned char> compressed;
    // Provide input
    c_stream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original.data()));
    // avail_in is uInt; cast is safe for typical small input
    c_stream.avail_in = static_cast<uInt>(original.size());

    unsigned char outbuf[CHUNK];
    // Call deflate in a loop until it returns Z_STREAM_END
    for (;;) {
        c_stream.next_out = outbuf;
        c_stream.avail_out = CHUNK;

        ret = deflate(&c_stream, Z_FINISH); // finish since we provided all input
        if (ret != Z_OK && ret != Z_STREAM_END) {
            deflateEnd(&c_stream);
            return 2; // deflate failed
        }

        const uInt produced = CHUNK - c_stream.avail_out;
        if (produced > 0) {
            compressed.insert(compressed.end(), outbuf, outbuf + produced);
        }

        if (ret == Z_STREAM_END) break;
        // otherwise continue calling deflate until Z_STREAM_END
    }

    // Clean up deflate
    if (deflateEnd(&c_stream) != Z_OK) {
        return 3; // deflateEnd failed
    }

    // --- Decompression (inflate) ---
    z_stream d_stream;
    std::memset(&d_stream, 0, sizeof(d_stream));
    ret = inflateInit(&d_stream);
    if (ret != Z_OK) {
        return 4; // inflateInit failed
    }

    std::vector<unsigned char> decompressed;
    d_stream.next_in = compressed.empty() ? Z_NULL : reinterpret_cast<Bytef*>(compressed.data());
    d_stream.avail_in = static_cast<uInt>(compressed.size());

    unsigned char inoutbuf[CHUNK];
    for (;;) {
        d_stream.next_out = inoutbuf;
        d_stream.avail_out = CHUNK;

        ret = inflate(&d_stream, Z_NO_FLUSH);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            inflateEnd(&d_stream);
            return 5; // inflate failed
        }

        const uInt have = CHUNK - d_stream.avail_out;
        if (have > 0) {
            decompressed.insert(decompressed.end(), inoutbuf, inoutbuf + have);
        }

        if (ret == Z_STREAM_END) break;
        // continue until we hit Z_STREAM_END
    }

    if (inflateEnd(&d_stream) != Z_OK) {
        return 6; // inflateEnd failed
    }

    // Verify decompressed result equals original
    const std::string result(reinterpret_cast<const char*>(decompressed.data()), decompressed.size());
    if (result != original) {
        return 7; // mismatch
    }

    // Optionally show use of a checksum API (crc32) to demonstrate another zlib API:
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(original.data()), static_cast<uInt>(original.size()));
    uLong crc_down = crc32(0L, Z_NULL, 0);
    crc_down = crc32(crc_down, reinterpret_cast<const Bytef*>(result.data()), static_cast<uInt>(result.size()));
    if (crc_orig != crc_down) {
        return 8; // checksum mismatch (shouldn't happen if contents match)
    }

    // All checks passed
    return 66;
}