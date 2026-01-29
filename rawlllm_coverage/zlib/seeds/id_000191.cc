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
//<ID> 191
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Prepare sample input data (repeated to make it reasonably large)
    std::string original_piece = "The quick brown fox jumps over the lazy dog. ";
    std::string original;
    for (int i = 0; i < 200; ++i) original += original_piece; // ~9000 bytes

    // 1) Simple one-shot compress/uncompress using compress2/uncompress
    uLong src_len = static_cast<uLong>(original.size());
    uLongf comp_bound = compressBound(src_len);
    std::vector<unsigned char> comp_buf(static_cast<size_t>(comp_bound));
    uLongf comp_size = comp_buf.size();

    int ret = compress2(comp_buf.data(), &comp_size,
                        reinterpret_cast<const Bytef*>(original.data()), src_len,
                        Z_BEST_COMPRESSION);
    if (ret != Z_OK) return -1; // compress failed

    // Prepare output buffer for uncompress (we know expected size)
    std::vector<unsigned char> uncompress_buf(static_cast<size_t>(src_len));
    uLongf uncompress_size = src_len;
    ret = uncompress(uncompress_buf.data(), &uncompress_size,
                     comp_buf.data(), comp_size);
    if (ret != Z_OK) return -2; // uncompress failed
    if (uncompress_size != src_len) return -3; // size mismatch
    if (std::memcmp(uncompress_buf.data(), original.data(), src_len) != 0) return -4; // data mismatch

    // 2) Streaming deflate/inflate using z_stream (deflateInit / deflate / deflateEnd and inflateInit / inflate / inflateEnd)
    // We'll compress 'original' again using deflate in streaming mode.
    z_stream defstrm;
    std::memset(&defstrm, 0, sizeof(defstrm));
    ret = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -5;

    // Set input
    defstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original.data()));
    defstrm.avail_in = static_cast<uInt>(src_len);

    // Reserve an output buffer (use compressBound as safe upper bound)
    std::vector<unsigned char> stream_comp_buf(static_cast<size_t>(compressBound(src_len)));
    defstrm.next_out = stream_comp_buf.data();
    defstrm.avail_out = static_cast<uInt>(stream_comp_buf.size());

    // Perform deflate with Z_FINISH to finish the stream in one call (output buffer is large enough)
    ret = deflate(&defstrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // try to end and cleanup before returning
        deflateEnd(&defstrm);
        return -6;
    }
    // total_out holds compressed size
    uLong stream_comp_size = defstrm.total_out;
    deflateEnd(&defstrm);

    // Now inflate the streaming-compressed data
    z_stream infstrm;
    std::memset(&infstrm, 0, sizeof(infstrm));
    ret = inflateInit(&infstrm);
    if (ret != Z_OK) return -7;

    infstrm.next_in = stream_comp_buf.data();
    infstrm.avail_in = static_cast<uInt>(stream_comp_size);

    // Prepare output buffer sized to original (we expect exact original size)
    std::vector<unsigned char> stream_uncomp_buf(static_cast<size_t>(src_len));
    infstrm.next_out = stream_uncomp_buf.data();
    infstrm.avail_out = static_cast<uInt>(stream_uncomp_buf.size());

    ret = inflate(&infstrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstrm);
        return -8;
    }
    uLong stream_uncomp_size = infstrm.total_out;
    inflateEnd(&infstrm);

    if (stream_uncomp_size != src_len) return -9;
    if (std::memcmp(stream_uncomp_buf.data(), original.data(), src_len) != 0) return -10;

    // 3) Use gz* API briefly: create a gz-compressed buffer in memory by compressing to a temporary file isn't possible here,
    // but demonstrate the API calls with gzopen/gzwrite/gzclose if a filename were available.
    // (We won't fail the test if these calls are not invoked with files - demonstrating usage through comments)
    // Example usage (commented-out because it requires filesystem):
    // gzFile gz = gzopen("temp_test.gz", "wb");
    // if (!gz) return -11;
    // int written = gzwrite(gz, original.data(), static_cast<unsigned int>(original.size()));
    // gzclose(gz);
    // if (written <= 0) return -12;

    // If all checks passed, return 66 as requested
    return 66;
}