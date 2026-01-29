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
//<ID> 102
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Prepare test data
    std::string input;
    for (int i = 0; i < 200; ++i) {
        input += "The quick brown fox jumps over the lazy dog. ";
    }
    const Bytef* input_data = reinterpret_cast<const Bytef*>(input.data());
    uLong input_len = static_cast<uLong>(input.size());

    // 1) Compute checksums (crc32 and adler32)
    uLong crc_before = crc32(0L, Z_NULL, 0);
    crc_before = crc32(crc_before, input_data, input_len);

    uLong adler_before = adler32(0L, Z_NULL, 0);
    adler_before = adler32(adler_before, input_data, input_len);

    // 2) One-shot compress/uncompress using compress2/uncompress
    uLong bound = compressBound(input_len);
    std::vector<Bytef> comp_buf(bound);
    uLongf comp_size = bound;
    int ret = compress2(comp_buf.data(), &comp_size, input_data, input_len, Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        return 1; // compress2 failed
    }

    std::vector<Bytef> restored_buf(input_len);
    uLongf restored_size = input_len;
    ret = uncompress(restored_buf.data(), &restored_size, comp_buf.data(), comp_size);
    if (ret != Z_OK || restored_size != input_len) {
        return 2; // uncompress failed
    }

    if (std::memcmp(restored_buf.data(), input_data, input_len) != 0) {
        return 3; // data mismatch after uncompress
    }

    // Verify checksums on restored data
    uLong crc_after = crc32(0L, Z_NULL, 0);
    crc_after = crc32(crc_after, restored_buf.data(), restored_size);
    if (crc_after != crc_before) {
        return 4; // crc mismatch
    }

    uLong adler_after = adler32(0L, Z_NULL, 0);
    adler_after = adler32(adler_after, restored_buf.data(), restored_size);
    if (adler_after != adler_before) {
        return 5; // adler mismatch
    }

    // 3) Streaming compress using deflateInit/deflate/deflateEnd
    z_stream defstrm;
    std::memset(&defstrm, 0, sizeof(defstrm));
    ret = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return 6; // deflateInit failed
    }

    defstrm.next_in = const_cast<Bytef*>(input_data);
    defstrm.avail_in = input_len;

    // allocate output buffer with compressBound (should be enough)
    std::vector<Bytef> stream_comp_buf(compressBound(input_len));
    defstrm.next_out = stream_comp_buf.data();
    defstrm.avail_out = static_cast<uInt>(stream_comp_buf.size());

    // call deflate with Z_FINISH to finish the stream
    ret = deflate(&defstrm, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        deflateEnd(&defstrm);
        return 7; // deflate failed
    }
    // total produced
    uLongf stream_comp_size = static_cast<uLongf>(stream_comp_buf.size() - defstrm.avail_out);

    // finish deflate
    ret = deflateEnd(&defstrm);
    if (ret != Z_OK) {
        return 8; // deflateEnd failed
    }

    // 4) Streaming decompress using inflateInit/inflate/inflateEnd
    z_stream infstrm;
    std::memset(&infstrm, 0, sizeof(infstrm));
    ret = inflateInit(&infstrm);
    if (ret != Z_OK) {
        return 9; // inflateInit failed
    }

    infstrm.next_in = stream_comp_buf.data();
    infstrm.avail_in = static_cast<uInt>(stream_comp_size);

    std::vector<Bytef> stream_restored_buf(input_len);
    infstrm.next_out = stream_restored_buf.data();
    infstrm.avail_out = static_cast<uInt>(stream_restored_buf.size());

    ret = inflate(&infstrm, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        inflateEnd(&infstrm);
        return 10; // inflate failed
    }

    uLongf stream_restored_size = static_cast<uLongf>(stream_restored_buf.size() - infstrm.avail_out);

    ret = inflateEnd(&infstrm);
    if (ret != Z_OK) {
        return 11; // inflateEnd failed
    }

    if (stream_restored_size != input_len) {
        return 12; // size mismatch after streaming decompress
    }
    if (std::memcmp(stream_restored_buf.data(), input_data, input_len) != 0) {
        return 13; // data mismatch after streaming decompress
    }

    // 5) Use crc32 on streaming-restored data to double-check
    uLong crc_stream = crc32(0L, Z_NULL, 0);
    crc_stream = crc32(crc_stream, stream_restored_buf.data(), stream_restored_size);
    if (crc_stream != crc_before) {
        return 14; // crc mismatch on streamed data
    }

    // All checks passed
    return 66;
}