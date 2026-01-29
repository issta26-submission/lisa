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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Original data to compress
    const std::string orig = std::string("The quick brown fox jumps over the lazy dog. ")
                           + std::string("The quick brown fox jumps over the lazy dog. ")
                           + std::string("The quick brown fox jumps over the lazy dog. "); // repeated to be compressible
    const size_t orig_len = orig.size();
    const Bytef* orig_buf = reinterpret_cast<const Bytef*>(orig.data());

    // 1) Compute Adler-32 and CRC32 on original data
    uLong adler = adler32(0L, Z_NULL, 0); // initialize
    adler = adler32(adler, orig_buf, static_cast<uInt>(orig_len));

    uLong crc = crc32(0L, Z_NULL, 0); // initialize
    crc = crc32(crc, orig_buf, static_cast<uInt>(orig_len));

    // Basic sanity: values should not be zero for a non-empty string (though zero is technically possible for some data).
    // We'll just ensure APIs ran successfully by checking we can compress/decompress correctly.

    // 2) Single-call compression using compress2()
    uLongf comp_bound = compressBound(static_cast<uLong>(orig_len));
    std::vector<Bytef> comp_buf(comp_bound);
    uLongf comp_len = comp_bound;
    int ret = compress2(comp_buf.data(), &comp_len, orig_buf, static_cast<uLong>(orig_len), Z_BEST_COMPRESSION);
    if (ret != Z_OK) return -1; // compression failed

    comp_buf.resize(comp_len); // shrink to actual compressed size

    // 3) Single-call decompression using uncompress()
    std::vector<Bytef> uncompressed_buf(orig_len);
    uLongf uncompressed_len = static_cast<uLongf>(orig_len);
    ret = uncompress(uncompressed_buf.data(), &uncompressed_len, comp_buf.data(), comp_len);
    if (ret != Z_OK) return -2; // decompression failed
    if (uncompressed_len != orig_len) return -3;
    if (std::memcmp(uncompressed_buf.data(), orig_buf, orig_len) != 0) return -4; // data mismatch

    // 4) Streaming (deflate) compression using deflateInit / deflate / deflateEnd
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -5;

    defstream.next_in = const_cast<Bytef*>(orig_buf);
    defstream.avail_in = static_cast<uInt>(orig_len);

    const size_t CHUNK = 128;
    std::vector<Bytef> stream_comp;
    int def_ret = Z_OK;
    while (true) {
        // ensure space for output chunk
        size_t before = stream_comp.size();
        stream_comp.resize(before + CHUNK);
        defstream.next_out = stream_comp.data() + before;
        defstream.avail_out = static_cast<uInt>(CHUNK);

        def_ret = deflate(&defstream, Z_FINISH); // finish in one pass if small enough; loop handles partial output
        if (def_ret != Z_OK && def_ret != Z_STREAM_END) {
            deflateEnd(&defstream);
            return -6;
        }
        // how many bytes were produced
        size_t produced = CHUNK - defstream.avail_out;
        stream_comp.resize(before + produced);

        if (def_ret == Z_STREAM_END) break;
        // otherwise continue (should not loop infinitely since avail_in will reach zero and Z_FINISH will finish)
    }
    deflateEnd(&defstream);

    // 5) Streaming (inflate) decompression using inflateInit / inflate / inflateEnd
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    ret = inflateInit(&infstream);
    if (ret != Z_OK) return -7;

    infstream.next_in = stream_comp.data();
    infstream.avail_in = static_cast<uInt>(stream_comp.size());

    std::vector<Bytef> stream_uncomp;
    int inf_ret = Z_OK;
    while (true) {
        size_t before = stream_uncomp.size();
        stream_uncomp.resize(before + CHUNK);
        infstream.next_out = stream_uncomp.data() + before;
        infstream.avail_out = static_cast<uInt>(CHUNK);

        inf_ret = inflate(&infstream, Z_NO_FLUSH);
        if (inf_ret != Z_OK && inf_ret != Z_STREAM_END) {
            inflateEnd(&infstream);
            return -8;
        }
        size_t produced = CHUNK - infstream.avail_out;
        stream_uncomp.resize(before + produced);

        if (inf_ret == Z_STREAM_END) break;
        // continue until stream end
    }
    inflateEnd(&infstream);

    // Verify streaming decompression matches original
    if (stream_uncomp.size() != orig_len) return -9;
    if (std::memcmp(stream_uncomp.data(), orig_buf, orig_len) != 0) return -10;

    // 6) As an additional API usage, demonstrate compress() + uncompress() pair (compress uses default compression)
    std::vector<Bytef> comp2_buf(comp_bound);
    uLongf comp2_len = comp_bound;
    ret = compress(comp2_buf.data(), &comp2_len, orig_buf, static_cast<uLong>(orig_len));
    if (ret != Z_OK) return -11;

    std::vector<Bytef> uncomp2_buf(orig_len);
    uLongf uncomp2_len = static_cast<uLongf>(orig_len);
    ret = uncompress(uncomp2_buf.data(), &uncomp2_len, comp2_buf.data(), comp2_len);
    if (ret != Z_OK) return -12;
    if (uncomp2_len != orig_len) return -13;
    if (std::memcmp(uncomp2_buf.data(), orig_buf, orig_len) != 0) return -14;

    // If we reach here, all tested zlib API sequences succeeded.
    (void)adler; // silence unused-variable warnings if any static analysis complains
    (void)crc;
    return 66;
}