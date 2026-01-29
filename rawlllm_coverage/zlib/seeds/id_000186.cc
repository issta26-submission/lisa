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
//<ID> 186
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Necessary headers (allowed inside function to keep this snippet self-contained).
    // In a normal project these would be at the top of the file.
    #include <zlib.h>
    #include <cstring>
    #include <vector>
    #include <string>

    // Sample input data to compress/decompress
    const char sample[] =
        "The quick brown fox jumps over the lazy dog. "
        "zlib test sequence: compress/uncompress, deflate/inflate, crc32/adler32.";
    const std::size_t sample_len = std::strlen(sample);

    // 1) Compute CRC32 and Adler-32 checksums
    // crc32 and adler32 expect uInt lengths; our sample is small so cast is safe.
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(sample), static_cast<uInt>(sample_len));

    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(sample), static_cast<uInt>(sample_len));

    // 2) Use the convenience compress/uncompress API
    uLong bound = compressBound(static_cast<uLong>(sample_len));
    std::vector<Bytef> comp_buf(bound);
    uLongf comp_size = static_cast<uLongf>(bound);

    int c_ret = compress(comp_buf.data(), &comp_size,
                         reinterpret_cast<const Bytef*>(sample),
                         static_cast<uLong>(sample_len));
    if (c_ret != Z_OK) return -1; // compress failed

    // Uncompress into a buffer sized to original length
    std::vector<Bytef> uncomp_buf(sample_len);
    uLongf uncmp_size = static_cast<uLongf>(sample_len);
    int u_ret = uncompress(uncomp_buf.data(), &uncmp_size,
                           comp_buf.data(), comp_size);
    if (u_ret != Z_OK) return -2; // uncompress failed
    if (uncmp_size != sample_len) return -3; // unexpected uncompressed size
    if (std::memcmp(uncomp_buf.data(), sample, sample_len) != 0) return -4; // data mismatch

    // 3) Streaming API: deflate (compress) using z_stream
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    int dinit = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (dinit != Z_OK) return -5;

    // Provide input
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(sample));
    defstream.avail_in = static_cast<uInt>(sample_len);

    // Output buffer for streaming deflate
    std::vector<Bytef> stream_comp_buf( (sample_len > 128) ? sample_len + 128 : 1024 );
    defstream.next_out = stream_comp_buf.data();
    defstream.avail_out = static_cast<uInt>(stream_comp_buf.size());

    int dret = deflate(&defstream, Z_FINISH);
    if (dret != Z_STREAM_END) { deflateEnd(&defstream); return -6; }
    std::size_t stream_comp_size = stream_comp_buf.size() - defstream.avail_out;
    deflateEnd(&defstream);

    // 4) Streaming API: inflate (decompress) using z_stream
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    int iinit = inflateInit(&infstream);
    if (iinit != Z_OK) return -7;

    infstream.next_in = stream_comp_buf.data();
    infstream.avail_in = static_cast<uInt>(stream_comp_size);

    // Output buffer for streaming inflate
    std::vector<Bytef> stream_uncomp_buf(sample_len + 16); // a little extra room
    infstream.next_out = stream_uncomp_buf.data();
    infstream.avail_out = static_cast<uInt>(stream_uncomp_buf.size());

    int iret = inflate(&infstream, Z_FINISH);
    if (iret != Z_STREAM_END) { inflateEnd(&infstream); return -8; }
    std::size_t stream_uncomp_size = stream_uncomp_buf.size() - infstream.avail_out;
    inflateEnd(&infstream);

    if (stream_uncomp_size != sample_len) return -9;
    if (std::memcmp(stream_uncomp_buf.data(), sample, sample_len) != 0) return -10;

    // Cross-check: verify that CRC/Adler computed earlier still match on decompressed data
    uLong crc_check = crc32(0L, Z_NULL, 0);
    crc_check = crc32(crc_check, stream_uncomp_buf.data(), static_cast<uInt>(stream_uncomp_size));
    if (crc_check != crc) return -11;

    uLong adl_check = adler32(0L, Z_NULL, 0);
    adl_check = adler32(adl_check, stream_uncomp_buf.data(), static_cast<uInt>(stream_uncomp_size));
    if (adl_check != adl) return -12;

    // All tests succeeded; return the required success code.
    return 66;
}