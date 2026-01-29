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
//<ID> 145
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Source data to compress
    const char source[] = "The quick brown fox jumps over the lazy dog";
    const uLong source_len = static_cast<uLong>(std::strlen(source) + 1); // include terminating NUL

    // --- 1) Simple compress/uncompress using compress()/uncompress() ---

    // Allocate buffer for compressed data using compressBound()
    uLong bound = compressBound(source_len);
    std::vector<Bytef> comp_buf(bound);

    uLong comp_len = bound;
    int ret = compress(comp_buf.data(), &comp_len, reinterpret_cast<const Bytef*>(source), source_len);
    if (ret != Z_OK) {
        return 1; // compress failed
    }

    // Allocate buffer for uncompressed result
    std::vector<Bytef> unp_buf(source_len);
    uLong unp_len = source_len;
    ret = uncompress(unp_buf.data(), &unp_len, comp_buf.data(), comp_len);
    if (ret != Z_OK || unp_len != source_len) {
        return 2; // uncompress failed
    }

    // Verify data matches original
    if (std::memcmp(unp_buf.data(), source, source_len) != 0) {
        return 3; // data mismatch after uncompress
    }

    // --- 2) Streaming deflate/inflate using z_stream, deflateInit/deflate/inflateInit/inflate ---

    // Prepare deflate stream
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return 4; // deflateInit failed
    }

    // Set input
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(source));
    defstream.avail_in = source_len;

    // Output buffer for deflate (use compressBound for safe size)
    std::vector<Bytef> def_out_buf(compressBound(source_len));
    defstream.next_out = def_out_buf.data();
    defstream.avail_out = static_cast<uInt>(def_out_buf.size());

    // Run deflate with Z_FINISH to compress all input
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // If not finished, try to continue until Z_STREAM_END (unlikely with large enough buffer)
        while (ret == Z_OK) {
            // Not expecting more output in this test scenario, but handle generically
            uLong old_size = def_out_buf.size();
            def_out_buf.resize(old_size + 1024);
            defstream.next_out = def_out_buf.data() + defstream.total_out;
            defstream.avail_out = static_cast<uInt>(def_out_buf.size() - defstream.total_out);
            ret = deflate(&defstream, Z_FINISH);
            if (ret == Z_STREAM_END) break;
            if (ret != Z_OK) break;
        }
    }

    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return 5; // deflate did not finish correctly
    }

    // Total compressed size
    uLong deflated_size = defstream.total_out;

    // Clean up deflate
    ret = deflateEnd(&defstream);
    if (ret != Z_OK) {
        return 6; // deflateEnd failed
    }

    // Prepare inflate stream
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.next_in = def_out_buf.data();
    infstream.avail_in = static_cast<uInt>(deflated_size);

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return 7; // inflateInit failed
    }

    // Output buffer for inflate
    std::vector<Bytef> inflate_out_buf(source_len);
    infstream.next_out = inflate_out_buf.data();
    infstream.avail_out = static_cast<uInt>(inflate_out_buf.size());

    // Run inflate
    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // Try to continue if needed
        while (ret == Z_OK) {
            uLong old_size = inflate_out_buf.size();
            inflate_out_buf.resize(old_size + 1024);
            infstream.next_out = inflate_out_buf.data() + infstream.total_out;
            infstream.avail_out = static_cast<uInt>(inflate_out_buf.size() - infstream.total_out);
            ret = inflate(&infstream, Z_FINISH);
            if (ret == Z_STREAM_END) break;
            if (ret != Z_OK) break;
        }
    }

    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return 8; // inflate did not finish correctly
    }

    uLong inflated_size = infstream.total_out;

    // Clean up inflate
    ret = inflateEnd(&infstream);
    if (ret != Z_OK) {
        return 9; // inflateEnd failed
    }

    // Verify sizes and content match original
    if (inflated_size != source_len) {
        return 10; // unexpected inflated size
    }
    if (std::memcmp(inflate_out_buf.data(), source, source_len) != 0) {
        return 11; // mismatch after inflate
    }

    // All tests succeeded
    return 66;
}