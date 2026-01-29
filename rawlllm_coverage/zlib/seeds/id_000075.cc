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
//<ID> 75
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

// Returns 66 on success, otherwise a non-66 error code.
int test_zlib_api_sequence() {
    // Prepare sample input (repeat to make it reasonably sized)
    std::string base = "The quick brown fox jumps over the lazy dog. ";
    std::string input;
    for (int i = 0; i < 16; ++i) input += base; // ~720 bytes

    const Bytef* input_data = reinterpret_cast<const Bytef*>(input.data());
    uLong input_len = static_cast<uLong>(input.size());
    if (input_len == 0) return 1;

    // 1) Compute checksums (crc32 and adler32)
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, input_data, input_len);
    uLong adler = adler32(0L, Z_NULL, 0);
    adler = adler32(adler, input_data, input_len);

    // 2) One-shot compress/uncompress using compress2/uncompress
    uLongf comp_bound = compressBound(input_len);
    std::vector<Bytef> comp_buf(static_cast<size_t>(comp_bound));
    int ret = compress2(comp_buf.data(), &comp_bound, input_data, input_len, Z_BEST_COMPRESSION);
    if (ret != Z_OK) return 2;
    comp_buf.resize(static_cast<size_t>(comp_bound));

    std::vector<Bytef> uncomp_buf(static_cast<size_t>(input_len));
    uLongf uncmp_len = input_len;
    ret = uncompress(uncomp_buf.data(), &uncmp_len, comp_buf.data(), comp_buf.size());
    if (ret != Z_OK || uncmp_len != input_len) return 3;
    if (std::memcmp(uncomp_buf.data(), input_data, input_len) != 0) return 4;

    // 3) Streaming deflate (z_stream + deflateInit/deflate/deflateEnd)
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 5;

    // Use a buffer sized by compressBound for streaming output
    uLongf stream_out_bound = compressBound(input_len);
    std::vector<Bytef> stream_comp_buf(static_cast<size_t>(stream_out_bound));
    defstream.next_in = const_cast<Bytef*>(input_data);
    defstream.avail_in = static_cast<uInt>(input_len); // safe because input is small
    defstream.next_out = stream_comp_buf.data();
    defstream.avail_out = static_cast<uInt>(stream_comp_buf.size());
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return 6;
    }
    uLong stream_comp_size = defstream.total_out;
    deflateEnd(&defstream);
    stream_comp_buf.resize(static_cast<size_t>(stream_comp_size));

    // 4) Streaming inflate (z_stream + inflateInit/inflate/inflateEnd)
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.next_in = stream_comp_buf.data();
    infstream.avail_in = static_cast<uInt>(stream_comp_size);
    std::vector<Bytef> stream_uncomp_buf(static_cast<size_t>(input_len));
    infstream.next_out = stream_uncomp_buf.data();
    infstream.avail_out = static_cast<uInt>(stream_uncomp_buf.size());
    ret = inflateInit(&infstream);
    if (ret != Z_OK) return 7;
    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return 8;
    }
    uLong stream_out_size = infstream.total_out;
    inflateEnd(&infstream);
    if (stream_out_size != input_len) return 9;
    if (std::memcmp(stream_uncomp_buf.data(), input_data, input_len) != 0) return 10;

    // 5) Verify checksums match on decompressed data
    uLong crc_after = crc32(0L, Z_NULL, 0);
    crc_after = crc32(crc_after, stream_uncomp_buf.data(), stream_out_size);
    if (crc_after != crc) return 11;
    uLong adler_after = adler32(0L, Z_NULL, 0);
    adler_after = adler32(adler_after, stream_uncomp_buf.data(), stream_out_size);
    if (adler_after != adler) return 12;

    // 6) Query library version (zlibVersion) to ensure symbol usage
    const char* version = zlibVersion();
    if (version == nullptr) return 13;

    // All operations succeeded
    (void)version; // silence unused variable warnings if any
    return 66;
}