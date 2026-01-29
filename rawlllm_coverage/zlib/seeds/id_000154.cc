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
//<ID> 154
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include necessary headers here so the function is self-contained when inserted into a file.
    // Including inside a function is allowed by the preprocessor.
    #include <zlib.h>
    #include <cstring>
    #include <vector>
    #include <string>

    // Sample data to compress/decompress
    const char sample[] =
        "The quick brown fox jumps over the lazy dog. "
        "This is a test string for zlib compression and decompression. "
        "Repeat: The quick brown fox jumps over the lazy dog.";
    const uLong source_len = static_cast<uLong>(std::strlen(sample));

    // 1) Use the convenience API compress() / uncompress()
    {
        // Allocate buffer large enough for compressed data
        uLong bound = compressBound(source_len);
        std::vector<Bytef> comp_buf(bound);

        uLongf comp_len = bound;
        int rv = compress(comp_buf.data(), &comp_len, reinterpret_cast<const Bytef*>(sample), source_len);
        if (rv != Z_OK) {
            return -1; // compress() failed
        }

        // Decompress using uncompress()
        std::vector<Bytef> decomp_buf(source_len + 1); // +1 for safety/possible string terminator
        uLongf decomp_len = source_len;
        rv = uncompress(decomp_buf.data(), &decomp_len, comp_buf.data(), comp_len);
        if (rv != Z_OK) {
            return -2; // uncompress() failed
        }

        // Null-terminate and verify
        decomp_buf[decomp_len] = 0;
        if (std::strcmp(reinterpret_cast<char*>(decomp_buf.data()), sample) != 0) {
            return -3; // mismatch after uncompress
        }
    }

    // 2) Use the streaming API: deflateInit / deflate / deflateEnd and inflateInit / inflate / inflateEnd
    {
        // Prepare z_stream for deflate
        z_stream defstream;
        std::memset(&defstream, 0, sizeof(defstream));
        defstream.zalloc = Z_NULL;
        defstream.zfree = Z_NULL;
        defstream.opaque = Z_NULL;

        int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
        if (ret != Z_OK) {
            return -10; // deflateInit failed
        }

        // Output buffer size: compressBound is safe
        uLong bound = compressBound(source_len);
        std::vector<Bytef> outbuf(bound);
        defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(sample));
        defstream.avail_in = source_len;
        defstream.next_out = outbuf.data();
        defstream.avail_out = static_cast<uInt>(bound > 0xFFFFFFFFu ? 0xFFFFFFFFu : bound); // cap for uInt if needed

        // Perform deflation in one (or a few) calls
        ret = deflate(&defstream, Z_FINISH);
        if (ret != Z_STREAM_END && ret != Z_OK) {
            deflateEnd(&defstream);
            return -11; // deflate failed
        }
        // Number of bytes produced:
        uLong comp_size_stream = defstream.total_out;

        ret = deflateEnd(&defstream);
        if (ret != Z_OK) {
            return -12; // deflateEnd failed
        }

        // Now inflate
        z_stream infstream;
        std::memset(&infstream, 0, sizeof(infstream));
        infstream.zalloc = Z_NULL;
        infstream.zfree = Z_NULL;
        infstream.opaque = Z_NULL;

        ret = inflateInit(&infstream);
        if (ret != Z_OK) {
            return -20; // inflateInit failed
        }

        std::vector<Bytef> decomp_buf2(source_len + 1);
        infstream.next_in = outbuf.data();
        infstream.avail_in = static_cast<uInt>(comp_size_stream > 0xFFFFFFFFu ? 0xFFFFFFFFu : comp_size_stream);
        infstream.next_out = decomp_buf2.data();
        infstream.avail_out = static_cast<uInt>(decomp_buf2.size());

        ret = inflate(&infstream, Z_FINISH);
        if (ret != Z_STREAM_END && ret != Z_OK) {
            inflateEnd(&infstream);
            return -21; // inflate failed
        }
        uLong decomp_size_stream = infstream.total_out;

        ret = inflateEnd(&infstream);
        if (ret != Z_OK) {
            return -22; // inflateEnd failed
        }

        // Null-terminate and verify
        if (decomp_size_stream > 0 && decomp_size_stream < decomp_buf2.size()) {
            decomp_buf2[decomp_size_stream] = 0;
        } else if (decomp_size_stream == 0) {
            decomp_buf2[0] = 0;
        } else {
            return -23; // unexpected decompressed size
        }

        if (std::strcmp(reinterpret_cast<char*>(decomp_buf2.data()), sample) != 0) {
            return -24; // mismatch after inflate
        }
    }

    // If we reach here, both compress/uncompress and deflate/inflate sequences succeeded.
    return 66;
}