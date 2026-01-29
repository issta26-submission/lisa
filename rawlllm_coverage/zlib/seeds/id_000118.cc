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
//<ID> 118
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates a correct usage sequence of zlib APIs:
    // - Initialize a deflate stream, compress data in a loop, finish and end the stream.
    // - Initialize an inflate stream, decompress data in a loop, finish and end the stream.
    // - Verify decompressed data equals the original input.
    //
    // On success returns 66 as requested. On any error returns a non-66 value.

    const std::string original =
        "The quick brown fox jumps over the lazy dog. "
        "Repeat to make the payload a bit larger: "
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog.";

    // --- 1) Deflate (compress) using z_stream, deflateInit, deflate, deflateEnd ---
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        // deflateInit failed
        return -1;
    }

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original.data()));
    defstream.avail_in = static_cast<uInt>(original.size());

    std::vector<unsigned char> compressed;
    const size_t CHUNK = 128;
    unsigned char outbuf[CHUNK];

    bool deflate_finished = false;
    while (!deflate_finished) {
        defstream.next_out = outbuf;
        defstream.avail_out = static_cast<uInt>(CHUNK);

        int flush = (defstream.avail_in > 0) ? Z_NO_FLUSH : Z_FINISH;
        ret = deflate(&defstream, flush); // compress
        if (ret == Z_STREAM_ERROR) {
            deflateEnd(&defstream);
            return -2;
        }
        // number of bytes produced:
        size_t have = CHUNK - defstream.avail_out;
        if (have > 0) {
            compressed.insert(compressed.end(), outbuf, outbuf + have);
        }

        if (ret == Z_STREAM_END) {
            deflate_finished = true;
        }
        // Continue until Z_STREAM_END
    }

    // Finish and free deflate state
    ret = deflateEnd(&defstream);
    if (ret != Z_OK) {
        return -3;
    }

    // --- 2) Inflate (decompress) using z_stream, inflateInit, inflate, inflateEnd ---
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return -4;
    }

    infstream.next_in = compressed.empty() ? Z_NULL : reinterpret_cast<Bytef*>(compressed.data());
    infstream.avail_in = static_cast<uInt>(compressed.size());

    std::vector<unsigned char> decompressed;
    unsigned char inbuf_out[CHUNK];

    bool inflate_finished = false;
    while (!inflate_finished) {
        infstream.next_out = inbuf_out;
        infstream.avail_out = static_cast<uInt>(CHUNK);

        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            inflateEnd(&infstream);
            return -5;
        }

        size_t have = CHUNK - infstream.avail_out;
        if (have > 0) {
            decompressed.insert(decompressed.end(), inbuf_out, inbuf_out + have);
        }

        if (ret == Z_STREAM_END) {
            inflate_finished = true;
        }

        // If input consumed but not finished, loop will continue with avail_in==0 and inflate will
        // return Z_STREAM_END when it processes the end of the stream; otherwise keep looping.
        if (infstream.avail_in == 0 && ret == Z_OK && infstream.avail_out > 0) {
            // if no progress and no avail_in, break to avoid infinite loop
            // (normally should not happen because compressed data contains proper end)
            break;
        }
    }

    ret = inflateEnd(&infstream);
    if (ret != Z_OK) {
        return -6;
    }

    // --- 3) Verify equality ---
    std::string recovered(decompressed.begin(), decompressed.end());
    if (recovered != original) {
        return -7;
    }

    // Success: return the requested value
    return 66;
}