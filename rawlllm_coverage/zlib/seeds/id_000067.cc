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
//<ID> 67
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample input data to compress and then decompress
    const char input_data[] =
        "The quick brown fox jumps over the lazy dog. "
        "zlib test sequence: compress, then decompress, verify integrity.";
    const size_t input_len = strlen(input_data);

    // ---- Deflate (compress) ----
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input_data));
    defstream.avail_in = static_cast<uInt>(input_len);

    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        // init failed
        return -1;
    }

    std::vector<unsigned char> compressed;
    const size_t CHUNK = 128;
    unsigned char outbuf[CHUNK];

    // Feed all input and finish
    while (true) {
        defstream.next_out = outbuf;
        defstream.avail_out = CHUNK;

        int flush = (defstream.avail_in > 0) ? Z_NO_FLUSH : Z_FINISH;
        ret = deflate(&defstream, flush);
        if (ret == Z_STREAM_ERROR) {
            deflateEnd(&defstream);
            return -2;
        }

        size_t have = CHUNK - defstream.avail_out;
        if (have > 0) {
            compressed.insert(compressed.end(), outbuf, outbuf + have);
        }

        if (ret == Z_STREAM_END) break;
        // otherwise continue until Z_STREAM_END
    }

    deflateEnd(&defstream);

    // ---- Inflate (decompress) ----
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    infstream.next_in = compressed.empty() ? Z_NULL : reinterpret_cast<Bytef*>(compressed.data());
    infstream.avail_in = static_cast<uInt>(compressed.size());

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return -3;
    }

    std::vector<unsigned char> decompressed;
    unsigned char inbuf[CHUNK];

    while (true) {
        infstream.next_out = inbuf;
        infstream.avail_out = CHUNK;

        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            inflateEnd(&infstream);
            return -4;
        }

        size_t have = CHUNK - infstream.avail_out;
        if (have > 0) {
            decompressed.insert(decompressed.end(), inbuf, inbuf + have);
        }

        if (ret == Z_STREAM_END) break;
        // If no progress and not ended, fail
        if (infstream.avail_in == 0 && infstream.avail_out == CHUNK) {
            inflateEnd(&infstream);
            return -5;
        }
    }

    inflateEnd(&infstream);

    // ---- Verify ----
    if (decompressed.size() != input_len) {
        return -6;
    }
    if (std::memcmp(decompressed.data(), input_data, input_len) != 0) {
        return -7;
    }

    // Success: return 66 as requested
    return 66;
}