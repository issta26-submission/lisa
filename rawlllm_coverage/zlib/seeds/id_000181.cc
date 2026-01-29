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
//<ID> 181
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

// Compress with compress()/uncompress() and then with deflate()/inflate()
// Verify round-trip equality. Return 66 on success, non-66 on failure.
int test_zlib_api_sequence() {
    // Prepare input data (repeated to make compression effective)
    std::string input;
    for (int i = 0; i < 100; ++i) {
        input += "The quick brown fox jumps over the lazy dog. ";
    }
    uLong input_len = static_cast<uLong>(input.size());

    // --- 1) High-level compress()/uncompress() API usage ---
    uLong bound = compressBound(input_len);
    std::vector<unsigned char> comp_buf(bound);
    uLong comp_len = bound;

    int ret = compress(comp_buf.data(), &comp_len,
                       reinterpret_cast<const Bytef*>(input.data()), input_len);
    if (ret != Z_OK) {
        return -1; // compress() failed
    }

    std::vector<unsigned char> decomp_buf(input_len);
    uLong decomp_len = input_len;
    ret = uncompress(decomp_buf.data(), &decomp_len,
                     comp_buf.data(), comp_len);
    if (ret != Z_OK || decomp_len != input_len) {
        return -2; // uncompress() failed or length mismatch
    }

    if (std::memcmp(decomp_buf.data(), input.data(), input_len) != 0) {
        return -3; // data mismatch after uncompress()
    }

    // --- 2) Lower-level deflate()/inflate() API usage with z_stream ---
    // Deflate (compress)
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return -4; // deflateInit failed
    }

    // Provide input
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = input_len;

    // Prepare output buffer (use compressBound to be safe)
    uLong deflate_bound = compressBound(input_len);
    std::vector<unsigned char> deflated(deflate_bound);
    defstream.next_out = deflated.data();
    defstream.avail_out = deflate_bound;

    // Run deflate with Z_FINISH; loop in case more output is needed
    while (true) {
        ret = deflate(&defstream, Z_FINISH);
        if (ret == Z_STREAM_END) break;
        if (ret != Z_OK && ret != Z_BUF_ERROR) {
            deflateEnd(&defstream);
            return -5; // deflate() failed
        }
        // If output buffer was not large enough, reallocate and continue
        if (defstream.avail_out == 0) {
            size_t old_size = deflated.size();
            deflated.resize(old_size * 2);
            defstream.next_out = deflated.data() + old_size;
            defstream.avail_out = static_cast<uInt>(deflated.size() - old_size);
        }
    }

    uLong deflated_size = deflate_bound - defstream.avail_out;
    deflateEnd(&defstream);

    // Inflate (decompress)
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return -6; // inflateInit failed
    }

    infstream.next_in = deflated.data();
    infstream.avail_in = static_cast<uInt>(deflated_size);

    std::vector<unsigned char> inflated(input_len);
    infstream.next_out = inflated.data();
    infstream.avail_out = static_cast<uInt>(input_len);

    while (true) {
        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) break;
        if (ret != Z_OK) {
            inflateEnd(&infstream);
            return -7; // inflate() failed
        }
        // If output buffer not large enough (shouldn't be for known size), handle:
        if (infstream.avail_out == 0) {
            size_t old_size = inflated.size();
            inflated.resize(old_size * 2);
            infstream.next_out = inflated.data() + old_size;
            infstream.avail_out = static_cast<uInt>(inflated.size() - old_size);
        }
    }

    uLong inflated_size = input_len - infstream.avail_out;
    inflateEnd(&infstream);

    if (inflated_size != input_len) {
        return -8; // inflated length mismatch
    }
    if (std::memcmp(inflated.data(), input.data(), input_len) != 0) {
        return -9; // data mismatch after inflate()
    }

    // If we reached here, both high-level and low-level sequences succeeded.
    return 66;
}