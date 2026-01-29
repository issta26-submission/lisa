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
//<ID> 143
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers required for zlib APIs and basic C functions.
    // (Including inside the function is valid since the preprocessor runs before compilation.)
    #include <zlib.h>
    #include <cstring>
    #include <cstdio>
    #include <cstdlib>

    // Sample input data (repeated to make a non-trivial buffer).
    const char *sample_fragment = "The quick brown fox jumps over the lazy dog. ";
    size_t fragment_len = std::strlen(sample_fragment);
    const int repeat = 16; // make input larger
    size_t src_len = fragment_len * repeat;

    // Build the input buffer by repeating the fragment.
    unsigned char *src = (unsigned char*)std::malloc(src_len + 1);
    if (!src) return -1;
    for (int i = 0; i < repeat; ++i) {
        std::memcpy(src + i * fragment_len, sample_fragment, fragment_len);
    }
    src[src_len] = '\0';

    // 1) Simple compress()/uncompress() usage.
    uLongf comp_bound = compressBound((uLong)src_len);
    Bytef *comp_buf = (Bytef*)std::malloc(comp_bound);
    if (!comp_buf) { std::free(src); return -2; }
    uLongf comp_len = comp_bound;
    int zret = compress(comp_buf, &comp_len, src, (uLong)src_len);
    if (zret != Z_OK) {
        std::free(src); std::free(comp_buf);
        return -3;
    }

    // Uncompress back
    Bytef *decomp_buf = (Bytef*)std::malloc(src_len + 1);
    if (!decomp_buf) { std::free(src); std::free(comp_buf); return -4; }
    uLongf decomp_len = (uLongf)src_len;
    zret = uncompress(decomp_buf, &decomp_len, comp_buf, comp_len);
    if (zret != Z_OK || decomp_len != src_len || std::memcmp(decomp_buf, src, src_len) != 0) {
        std::free(src); std::free(comp_buf); std::free(decomp_buf);
        return -5;
    }
    // Clean up buffers from compress/uncompress test.
    std::free(comp_buf);
    std::free(decomp_buf);

    // 2) Stream-based deflate/inflate (z_stream).
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    zret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (zret != Z_OK) { std::free(src); return -6; }

    // Prepare output buffer large enough.
    uLongf stream_out_bound = compressBound((uLong)src_len);
    Bytef *stream_comp = (Bytef*)std::malloc(stream_out_bound);
    if (!stream_comp) { deflateEnd(&defstream); std::free(src); return -7; }

    defstream.next_in = src;
    defstream.avail_in = (uInt)src_len; // safe because src_len is reasonable
    defstream.next_out = stream_comp;
    defstream.avail_out = (uInt)stream_out_bound;

    zret = deflate(&defstream, Z_FINISH);
    if (zret != Z_STREAM_END) {
        // cleanup
        deflateEnd(&defstream);
        std::free(stream_comp);
        std::free(src);
        return -8;
    }
    uLongf stream_comp_len = stream_out_bound - defstream.avail_out;
    deflateEnd(&defstream);

    // Inflate the stream_comp back
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    zret = inflateInit(&infstream);
    if (zret != Z_OK) {
        std::free(stream_comp); std::free(src);
        return -9;
    }

    Bytef *stream_decomp = (Bytef*)std::malloc(src_len + 1);
    if (!stream_decomp) { inflateEnd(&infstream); std::free(stream_comp); std::free(src); return -10; }

    infstream.next_in = stream_comp;
    infstream.avail_in = (uInt)stream_comp_len;
    infstream.next_out = stream_decomp;
    infstream.avail_out = (uInt)src_len;

    zret = inflate(&infstream, Z_FINISH);
    if (zret != Z_STREAM_END || (src_len - infstream.avail_out) != src_len ||
        std::memcmp(stream_decomp, src, src_len) != 0) {
        inflateEnd(&infstream);
        std::free(stream_comp); std::free(stream_decomp); std::free(src);
        return -11;
    }
    inflateEnd(&infstream);

    // Clean up stream-based buffers
    std::free(stream_comp);
    std::free(stream_decomp);

    // 3) gzFile API: write and read .gz file
    const char *gz_filename = "test_zlib_api_sequence.gz";
    gzFile gz = gzopen(gz_filename, "wb");
    if (!gz) { std::free(src); return -12; }
    int written = gzwrite(gz, src, (unsigned int)src_len);
    if (written == 0) { gzclose(gz); std::free(src); return -13; }
    gzclose(gz);

    gz = gzopen(gz_filename, "rb");
    if (!gz) { std::free(src); return -14; }
    unsigned char *gz_read_buf = (unsigned char*)std::malloc(src_len + 1);
    if (!gz_read_buf) { gzclose(gz); std::free(src); return -15; }
    int read_bytes = gzread(gz, gz_read_buf, (unsigned int)src_len);
    if (read_bytes <= 0 || (size_t)read_bytes != src_len || std::memcmp(gz_read_buf, src, src_len) != 0) {
        gzclose(gz);
        std::free(gz_read_buf);
        std::free(src);
        return -16;
    }
    gzclose(gz);
    std::free(gz_read_buf);

    // Optionally remove the temporary gz file (ignore remove error)
    std::remove(gz_filename);

    // 4) Checksum functions: crc32 and adler32
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, src, (uInt)src_len);

    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, src, (uInt)src_len);

    // Basic sanity checks: non-zero checksums for non-empty input
    if (crc == 0 || adl == 0) {
        std::free(src);
        return -17;
    }

    // Clean up input buffer and return success code 66.
    std::free(src);
    (void)crc; // silence unused-variable warnings if any static analysis complains
    (void)adl;
    return 66;
}