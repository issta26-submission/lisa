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
//<ID> 58
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

// A self-contained test function that exercises several common zlib APIs.
// Returns 66 on success, non-66 on failure.
int test_zlib_api_sequence() {
    // Prepare some compressible input data
    std::string base = "The quick brown fox jumps over the lazy dog. ";
    std::string input;
    for (int i = 0; i < 1000; ++i) input += base; // moderately large, repetitive data

    // --- 1) Single-shot compress / uncompress (compress(), uncompress(), compressBound()) ---
    uLong source_len = static_cast<uLong>(input.size());
    uLong bound = compressBound(source_len);
    std::vector<unsigned char> comp_buf(bound);
    uLongf comp_size = static_cast<uLongf>(bound);

    int ret = compress(reinterpret_cast<Bytef*>(comp_buf.data()),
                       &comp_size,
                       reinterpret_cast<const Bytef*>(input.data()),
                       source_len);
    if (ret != Z_OK) {
        return 1; // compress failed
    }
    comp_buf.resize(comp_size);

    // Uncompress back
    std::vector<unsigned char> uncomp_buf(source_len);
    uLongf uncmp_size = source_len;
    ret = uncompress(reinterpret_cast<Bytef*>(uncomp_buf.data()),
                     &uncmp_size,
                     reinterpret_cast<const Bytef*>(comp_buf.data()),
                     comp_size);
    if (ret != Z_OK || uncmp_size != source_len) {
        return 2; // uncompress failed
    }
    if (std::memcmp(uncomp_buf.data(), input.data(), source_len) != 0) {
        return 3; // data mismatch after uncompress
    }

    // --- 2) Stream (deflate/inflate) API usage ---
    const size_t CHUNK = 16384;
    std::vector<unsigned char> deflated_stream;
    deflated_stream.reserve(comp_size); // guess

    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    bool def_ok = false;
    if (deflateInit(&defstream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 4; // deflateInit failed
    }
    def_ok = true;

    defstream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(input.data()));
    defstream.avail_in = static_cast<uInt>(source_len);

    int flush = Z_FINISH; // we'll provide all input and ask for finish
    do {
        unsigned char outbuf[CHUNK];
        defstream.next_out = outbuf;
        defstream.avail_out = CHUNK;

        ret = deflate(&defstream, flush);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            deflateEnd(&defstream);
            return 5; // deflate error
        }

        size_t have = CHUNK - defstream.avail_out;
        deflated_stream.insert(deflated_stream.end(), outbuf, outbuf + have);
    } while (ret != Z_STREAM_END);

    deflateEnd(&defstream);
    def_ok = false;

    // Inflate (stream) back
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    if (inflateInit(&infstream) != Z_OK) {
        return 6; // inflateInit failed
    }

    infstream.next_in = deflated_stream.empty() ? Z_NULL : reinterpret_cast<Bytef*>(deflated_stream.data());
    infstream.avail_in = static_cast<uInt>(deflated_stream.size());

    std::vector<unsigned char> inflated_stream;
    inflated_stream.reserve(source_len);

    do {
        unsigned char outbuf[CHUNK];
        infstream.next_out = outbuf;
        infstream.avail_out = CHUNK;

        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            inflateEnd(&infstream);
            return 7; // inflate error
        }

        size_t have = CHUNK - infstream.avail_out;
        inflated_stream.insert(inflated_stream.end(), outbuf, outbuf + have);
    } while (ret != Z_STREAM_END);

    inflateEnd(&infstream);

    if (inflated_stream.size() != static_cast<size_t>(source_len) ||
        std::memcmp(inflated_stream.data(), input.data(), source_len) != 0) {
        return 8; // stream inflate produced wrong result
    }

    // --- 3) Checksum APIs: crc32 and adler32 ---
    uLong crc_initial = crc32(0L, Z_NULL, 0);
    uLong crc_val = crc32(crc_initial,
                          reinterpret_cast<const Bytef*>(input.data()),
                          source_len);

    uLong adler_initial = adler32(0L, Z_NULL, 0);
    uLong adler_val = adler32(adler_initial,
                              reinterpret_cast<const Bytef*>(input.data()),
                              source_len);

    // Verify checksums against decompressed data
    uLong crc_after = crc32(0L, Z_NULL, 0);
    crc_after = crc32(crc_after, reinterpret_cast<const Bytef*>(inflated_stream.data()), source_len);
    if (crc_after != crc_val) {
        return 9; // CRC mismatch
    }

    uLong adler_after = adler32(0L, Z_NULL, 0);
    adler_after = adler32(adler_after, reinterpret_cast<const Bytef*>(inflated_stream.data()), source_len);
    if (adler_after != adler_val) {
        return 10; // Adler-32 mismatch
    }

    // All checks passed, return the required 66
    return 66;
}