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
//<ID> 164
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample data to compress/decompress
    const char* sample = "The quick brown fox jumps over the lazy dog. "
                         "This sentence is repeated to give enough data for compression. "
                         "The quick brown fox jumps over the lazy dog. "
                         "The quick brown fox jumps over the lazy dog.";
    uLong orig_len = static_cast<uLong>(std::strlen(sample));

    // 1) Simple one-shot compression with compress2()
    uLong comp_bound = compressBound(orig_len);
    std::vector<Bytef> comp_buf(comp_bound);
    uLong comp_len = comp_bound;
    if (compress2(comp_buf.data(), &comp_len,
                  reinterpret_cast<const Bytef*>(sample), orig_len,
                  Z_BEST_COMPRESSION) != Z_OK) {
        return -1;
    }

    // 2) Verify checksum helpers: adler32 and crc32
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(sample), orig_len);
    (void)adl; // just exercise the API; value not used further

    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(sample), orig_len);
    (void)crc;

    // 3) One-shot uncompress() and verify content
    std::vector<Bytef> uncomp_buf(orig_len + 1);
    uLong uncmp_len = orig_len;
    if (uncompress(uncomp_buf.data(), &uncmp_len, comp_buf.data(), comp_len) != Z_OK) {
        return -2;
    }
    if (uncmp_len != orig_len ||
        std::memcmp(uncomp_buf.data(), sample, orig_len) != 0) {
        return -3;
    }

    // 4) Streaming compression using deflateInit/deflate/deflateEnd
    z_stream defstrm;
    std::memset(&defstrm, 0, sizeof(defstrm));
    if (deflateInit(&defstrm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return -4;
    }

    defstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(sample));
    defstrm.avail_in = static_cast<uInt>(orig_len);

    std::vector<Bytef> stream_comp_buf(compressBound(orig_len));
    defstrm.next_out = stream_comp_buf.data();
    defstrm.avail_out = static_cast<uInt>(stream_comp_buf.size());

    int dret = deflate(&defstrm, Z_FINISH);
    if (dret != Z_STREAM_END) {
        deflateEnd(&defstrm);
        return -5;
    }
    uLong stream_comp_len = static_cast<uLong>(stream_comp_buf.size()) - defstrm.avail_out;
    deflateEnd(&defstrm);

    // 5) Streaming decompression using inflateInit/inflate/inflateEnd
    z_stream infstrm;
    std::memset(&infstrm, 0, sizeof(infstrm));
    if (inflateInit(&infstrm) != Z_OK) {
        return -6;
    }

    infstrm.next_in = stream_comp_buf.data();
    infstrm.avail_in = static_cast<uInt>(stream_comp_len);

    std::vector<Bytef> stream_uncomp_buf(orig_len + 1);
    infstrm.next_out = stream_uncomp_buf.data();
    infstrm.avail_out = static_cast<uInt>(stream_uncomp_buf.size());

    int iret = inflate(&infstrm, Z_FINISH);
    if (iret != Z_STREAM_END) {
        inflateEnd(&infstrm);
        return -7;
    }
    uLong stream_uncomp_len = static_cast<uLong>(stream_uncomp_buf.size()) - infstrm.avail_out;
    inflateEnd(&infstrm);

    if (stream_uncomp_len != orig_len ||
        std::memcmp(stream_uncomp_buf.data(), sample, orig_len) != 0) {
        return -8;
    }

    // 6) gz* API: write compressed file and read it back
    const char* fname = "test_zlib_api_sequence_temp.gz";
    gzFile gzf = gzopen(fname, "wb");
    if (!gzf) return -9;

    int wrote = gzwrite(gzf, sample, static_cast<unsigned int>(orig_len));
    if (wrote == 0) { gzclose(gzf); std::remove(fname); return -10; }
    gzclose(gzf);

    gzFile gzf_read = gzopen(fname, "rb");
    if (!gzf_read) { std::remove(fname); return -11; }

    std::vector<char> read_buf(orig_len + 1);
    int read_bytes = gzread(gzf_read, read_buf.data(), static_cast<unsigned int>(orig_len));
    gzclose(gzf_read);

    if (read_bytes < 0) { std::remove(fname); return -12; }
    if (static_cast<uLong>(read_bytes) != orig_len ||
        std::memcmp(read_buf.data(), sample, orig_len) != 0) {
        std::remove(fname);
        return -13;
    }

    // Cleanup temporary file
    std::remove(fname);

    // All checks passed
    return 66;
}