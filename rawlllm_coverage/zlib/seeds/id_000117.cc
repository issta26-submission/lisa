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
//<ID> 117
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample data
    const unsigned char original[] = "The quick brown fox jumps over the lazy dog";
    const uLong orig_len = static_cast<uLong>(std::strlen(reinterpret_cast<const char*>(original)));

    // 1) compress2 + uncompress using compressBound
    uLongf comp_bound = compressBound(orig_len);
    std::vector<unsigned char> comp_buf(comp_bound);
    uLongf comp_len = comp_bound;
    int rc = compress2(comp_buf.data(), &comp_len, original, orig_len, Z_BEST_COMPRESSION);
    if (rc != Z_OK) return 1; // compress2 failed

    std::vector<unsigned char> decomp_buf(orig_len);
    uLongf decomp_len = orig_len;
    rc = uncompress(decomp_buf.data(), &decomp_len, comp_buf.data(), comp_len);
    if (rc != Z_OK || decomp_len != orig_len) return 2; // uncompress failed
    if (std::memcmp(decomp_buf.data(), original, orig_len) != 0) return 3; // mismatch

    // 2) crc32 and adler32 checks
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, original, orig_len);
    uLong crc_decomp = crc32(0L, Z_NULL, 0);
    crc_decomp = crc32(crc_decomp, decomp_buf.data(), decomp_len);
    if (crc_orig != crc_decomp) return 4;

    uLong adler_orig = adler32(0L, Z_NULL, 0);
    adler_orig = adler32(adler_orig, original, orig_len);
    uLong adler_decomp = adler32(0L, Z_NULL, 0);
    adler_decomp = adler32(adler_decomp, decomp_buf.data(), decomp_len);
    if (adler_orig != adler_decomp) return 5;

    // 3) deflate (stream) + inflate (stream)
    {
        z_stream def_strm;
        std::memset(&def_strm, 0, sizeof(def_strm));
        def_strm.zalloc = Z_NULL;
        def_strm.zfree = Z_NULL;
        def_strm.opaque = Z_NULL;
        if (deflateInit(&def_strm, Z_DEFAULT_COMPRESSION) != Z_OK) return 6;

        // ensure enough space using compressBound
        std::vector<unsigned char> stream_comp_buf(comp_bound);
        def_strm.next_in = const_cast<Bytef*>(original);
        def_strm.avail_in = orig_len;
        def_strm.next_out = stream_comp_buf.data();
        def_strm.avail_out = static_cast<uInt>(stream_comp_buf.size());

        int def_ret = deflate(&def_strm, Z_FINISH);
        if (def_ret != Z_STREAM_END) {
            deflateEnd(&def_strm);
            return 7;
        }
        uLong stream_comp_len = def_strm.total_out;
        deflateEnd(&def_strm);

        z_stream inf_strm;
        std::memset(&inf_strm, 0, sizeof(inf_strm));
        inf_strm.zalloc = Z_NULL;
        inf_strm.zfree = Z_NULL;
        inf_strm.opaque = Z_NULL;
        if (inflateInit(&inf_strm) != Z_OK) return 8;

        std::vector<unsigned char> stream_decomp_buf(orig_len);
        inf_strm.next_in = stream_comp_buf.data();
        inf_strm.avail_in = static_cast<uInt>(stream_comp_len);
        inf_strm.next_out = stream_decomp_buf.data();
        inf_strm.avail_out = static_cast<uInt>(stream_decomp_buf.size());

        int inf_ret = inflate(&inf_strm, Z_FINISH);
        if (inf_ret != Z_STREAM_END || inf_strm.total_out != orig_len) {
            inflateEnd(&inf_strm);
            return 9;
        }
        inflateEnd(&inf_strm);

        if (std::memcmp(stream_decomp_buf.data(), original, orig_len) != 0) return 10;
    }

    // 4) gzwrite + gzread to a temporary file
    const char* fname = "test_zlib_api_sequence_tmp.gz";
    {
        gzFile gz = gzopen(fname, "wb");
        if (!gz) return 11;
        int written = gzwrite(gz, original, static_cast<unsigned int>(orig_len));
        if (written != static_cast<int>(orig_len)) {
            gzclose(gz);
            remove(fname);
            return 12;
        }
        if (gzclose(gz) != Z_OK) {
            remove(fname);
            return 13;
        }

        gz = gzopen(fname, "rb");
        if (!gz) {
            remove(fname);
            return 14;
        }
        std::vector<unsigned char> gz_read_buf(orig_len);
        int read_bytes = gzread(gz, gz_read_buf.data(), static_cast<unsigned int>(orig_len));
        if (read_bytes != static_cast<int>(orig_len)) {
            gzclose(gz);
            remove(fname);
            return 15;
        }
        if (gzclose(gz) != Z_OK) {
            remove(fname);
            return 16;
        }
        if (std::memcmp(gz_read_buf.data(), original, orig_len) != 0) {
            remove(fname);
            return 17;
        }
        // remove temporary file
        remove(fname);
    }

    // 5) zlib version string (simple usage check)
    const char* ver = zlibVersion();
    if (!ver || ver[0] == '\0') return 18;

    // All checks passed
    return 66;
}