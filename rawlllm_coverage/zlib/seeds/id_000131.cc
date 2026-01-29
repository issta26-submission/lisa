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
//<ID> 131
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates a correct sequence of zlib APIs:
    //  - deflateInit -> deflate (feed data) -> deflate(..., Z_FINISH) -> deflateEnd
    //  - inflateInit -> inflate (recover) -> inflateEnd
    //
    // On success the function returns 66. On error it returns a non-66 value.
    
    #include <zlib.h>
    #include <string>
    #include <vector>
    #include <cstring>

    // Example input to compress
    std::string input;
    for (int i = 0; i < 50; ++i) {
        input += "The quick brown fox jumps over the lazy dog. ";
    }

    const size_t CHUNK = 16384;
    unsigned char temp[CHUNK];

    // --- Compression (deflate) ---
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return 1;
    }

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = static_cast<uInt>(input.size());

    std::vector<unsigned char> compressed;
    // First, feed the input
    while (defstream.avail_in > 0) {
        defstream.next_out = temp;
        defstream.avail_out = CHUNK;
        ret = deflate(&defstream, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR) {
            deflateEnd(&defstream);
            return 2;
        }
        size_t have = CHUNK - defstream.avail_out;
        compressed.insert(compressed.end(), temp, temp + have);
    }
    // Now finish the stream
    for (;;) {
        defstream.next_out = temp;
        defstream.avail_out = CHUNK;
        ret = deflate(&defstream, Z_FINISH);
        if (ret == Z_STREAM_ERROR) {
            deflateEnd(&defstream);
            return 3;
        }
        size_t have = CHUNK - defstream.avail_out;
        compressed.insert(compressed.end(), temp, temp + have);
        if (ret == Z_STREAM_END) break;
        // otherwise continue
    }

    deflateEnd(&defstream);

    // --- Decompression (inflate) ---
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    infstream.next_in = compressed.empty() ? Z_NULL : compressed.data();
    infstream.avail_in = static_cast<uInt>(compressed.size());

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return 4;
    }

    std::vector<unsigned char> decompressed;
    for (;;) {
        infstream.next_out = temp;
        infstream.avail_out = CHUNK;
        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            inflateEnd(&infstream);
            return 5;
        }
        size_t have = CHUNK - infstream.avail_out;
        decompressed.insert(decompressed.end(), temp, temp + have);
        if (ret == Z_STREAM_END) break;
        if (ret == Z_OK && infstream.avail_in == 0 && infstream.avail_out != 0) {
            // No more input and no error, wait for more or break if done
            // In this sequence we expect to finish with Z_STREAM_END above.
            // If we reach here unexpectedly, break to avoid infinite loop.
            break;
        }
    }

    inflateEnd(&infstream);

    // Compare decompressed data with original input
    std::string output(decompressed.begin(), decompressed.end());
    if (output != input) {
        return 6;
    }

    return 66;
}