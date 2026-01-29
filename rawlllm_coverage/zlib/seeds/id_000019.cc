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
//<ID> 19
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // include headers so this single function is self-contained for compilation
    #include <zlib.h>
    #include <vector>
    #include <string>
    #include <cstring>

    // Prepare sample input data (reasonably sized)
    const std::string input = std::string("The quick brown fox jumps over the lazy dog. ") + 
                              "This is a zlib API usage test. " +
                              std::string(200, 'x'); // make it a bit larger

    // 1) Compute checksums (crc32 and adler32) of the original data
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig,
                     reinterpret_cast<const Bytef*>(input.data()),
                     static_cast<uInt>(input.size()));
    uLong adler_orig = adler32(0L, Z_NULL, 0);
    adler_orig = adler32(adler_orig,
                         reinterpret_cast<const Bytef*>(input.data()),
                         static_cast<uInt>(input.size()));

    // 2) Use the simple one-shot compress/uncompress API
    uLongf bound = compressBound(static_cast<uLong>(input.size()));
    std::vector<Bytef> one_shot_comp(bound);
    uLongf comp_size = bound;
    int ret = compress(one_shot_comp.data(),
                       &comp_size,
                       reinterpret_cast<const Bytef*>(input.data()),
                       static_cast<uLong>(input.size()));
    if (ret != Z_OK) return -1;

    // Decompress with uncompress
    std::vector<Bytef> one_shot_decomp(input.size());
    uLongf decomp_size = static_cast<uLong>(one_shot_decomp.size());
    ret = uncompress(one_shot_decomp.data(),
                     &decomp_size,
                     one_shot_comp.data(),
                     comp_size);
    if (ret != Z_OK) return -2;
    if (decomp_size != input.size()) return -3;
    if (std::memcmp(one_shot_decomp.data(), input.data(), input.size()) != 0) return -4;

    // Verify checksums of decompressed data match originals
    uLong crc_after = crc32(0L, Z_NULL, 0);
    crc_after = crc32(crc_after,
                      one_shot_decomp.data(),
                      static_cast<uInt>(decomp_size));
    if (crc_after != crc_orig) return -5;
    uLong adler_after = adler32(0L, Z_NULL, 0);
    adler_after = adler32(adler_after,
                          one_shot_decomp.data(),
                          static_cast<uInt>(decomp_size));
    if (adler_after != adler_orig) return -6;

    // 3) Demonstrate stream-based deflate (compress) and inflate (decompress)
    // Deflate (stream)
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -7;

    defstream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(input.data()));
    defstream.avail_in = static_cast<uInt>(input.size());

    // Prepare a buffer and allow growth if needed
    std::size_t chunk = static_cast<std::size_t>(compressBound(static_cast<uLong>(input.size())));
    if (chunk == 0) chunk = 1024;
    std::vector<Bytef> stream_comp;
    stream_comp.resize(chunk);
    std::size_t total_out = 0;

    for (;;) {
        // Ensure there is space for output
        if (total_out == stream_comp.size()) {
            stream_comp.resize(stream_comp.size() + chunk);
        }
        defstream.next_out = stream_comp.data() + total_out;
        defstream.avail_out = static_cast<uInt>(stream_comp.size() - total_out);

        ret = deflate(&defstream, Z_FINISH);
        if (ret == Z_STREAM_END) {
            total_out = stream_comp.size() - defstream.avail_out;
            break;
        } else if (ret == Z_OK) {
            // produced some data but not finished; update total_out and loop to provide more space
            total_out = stream_comp.size() - defstream.avail_out;
            continue;
        } else {
            deflateEnd(&defstream);
            return -8;
        }
    }

    // finalize deflate
    if (deflateEnd(&defstream) != Z_OK) return -9;

    // Resize to actual compressed size
    stream_comp.resize(total_out);

    // Inflate (stream)
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) return -10;

    infstream.next_in = stream_comp.data();
    infstream.avail_in = static_cast<uInt>(stream_comp.size());

    std::vector<Bytef> stream_decomp;
    stream_decomp.resize(input.size()); // we expect original size
    std::size_t out_total = 0;

    for (;;) {
        if (out_total == stream_decomp.size()) {
            // should not be necessary for this example, but handle growth defensively
            stream_decomp.resize(stream_decomp.size() + input.size());
        }
        infstream.next_out = stream_decomp.data() + out_total;
        infstream.avail_out = static_cast<uInt>(stream_decomp.size() - out_total);

        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) {
            out_total = stream_decomp.size() - infstream.avail_out;
            break;
        } else if (ret == Z_OK) {
            out_total = stream_decomp.size() - infstream.avail_out;
            // continue inflating until end
            continue;
        } else {
            inflateEnd(&infstream);
            return -11;
        }
    }

    if (inflateEnd(&infstream) != Z_OK) return -12;

    // Trim to actual size
    stream_decomp.resize(out_total);

    // Verify the stream-decompressed data matches original
    if (out_total != input.size()) return -13;
    if (std::memcmp(stream_decomp.data(), input.data(), input.size()) != 0) return -14;

    // Verify checksums again on stream-decompressed data
    uLong crc_stream = crc32(0L, Z_NULL, 0);
    crc_stream = crc32(crc_stream, stream_decomp.data(), static_cast<uInt>(stream_decomp.size()));
    if (crc_stream != crc_orig) return -15;
    uLong adler_stream = adler32(0L, Z_NULL, 0);
    adler_stream = adler32(adler_stream, stream_decomp.data(), static_cast<uInt>(stream_decomp.size()));
    if (adler_stream != adler_orig) return -16;

    // 4) Optionally show usage of compress2 (same as compress but with level)
    {
        std::vector<Bytef> comp2_buf(compressBound(static_cast<uLong>(input.size())));
        uLongf comp2_len = static_cast<uLongf>(comp2_buf.size());
        ret = compress2(comp2_buf.data(),
                        &comp2_len,
                        reinterpret_cast<const Bytef*>(input.data()),
                        static_cast<uLong>(input.size()),
                        Z_BEST_SPEED);
        if (ret != Z_OK) return -17;
        // quick sanity check: decompress comp2_buf
        std::vector<Bytef> de2(input.size());
        uLongf dlen = static_cast<uLongf>(de2.size());
        ret = uncompress(de2.data(), &dlen, comp2_buf.data(), comp2_len);
        if (ret != Z_OK) return -18;
        if (dlen != input.size()) return -19;
        if (std::memcmp(de2.data(), input.data(), input.size()) != 0) return -20;
    }

    // If we reached here, all tests succeeded
    return 66;
}