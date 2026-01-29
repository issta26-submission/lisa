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
//<ID> 86
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // sample input
    const char* original = "The quick brown fox jumps over the lazy dog - zlib API sequence test.";
    size_t orig_len = strlen(original);

    // 1) crc32 and adler32 on original
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(original), static_cast<uInt>(orig_len));

    uLong adler_orig = adler32(0L, Z_NULL, 0);
    adler_orig = adler32(adler_orig, reinterpret_cast<const Bytef*>(original), static_cast<uInt>(orig_len));

    // 2) compress2 / uncompress
    uLong comp_bound = compressBound(static_cast<uLong>(orig_len));
    std::vector<Bytef> comp_buf(comp_bound);
    uLong comp_len = comp_bound;
    int cres = compress2(comp_buf.data(), &comp_len,
                         reinterpret_cast<const Bytef*>(original),
                         static_cast<uLong>(orig_len),
                         Z_BEST_COMPRESSION);
    if (cres != Z_OK) return 1;

    // uncompress
    std::vector<Bytef> uncompr_buf(orig_len);
    uLong uncompr_len = static_cast<uLong>(uncompr_buf.size());
    int ures = uncompress(uncompr_buf.data(), &uncompr_len, comp_buf.data(), comp_len);
    if (ures != Z_OK) return 2;
    if (uncompr_len != orig_len) return 3;
    if (memcmp(uncompr_buf.data(), original, orig_len) != 0) return 4;

    // 3) deflate / inflate via z_stream
    z_stream dstream;
    memset(&dstream, 0, sizeof(dstream));
    int ret = deflateInit(&dstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 5;

    dstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original));
    dstream.avail_in = static_cast<uInt>(orig_len);

    std::vector<Bytef> def_buf(compressBound(static_cast<uLong>(orig_len)));
    dstream.next_out = def_buf.data();
    dstream.avail_out = static_cast<uInt>(def_buf.size());

    ret = deflate(&dstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&dstream);
        return 6;
    }
    size_t def_size = def_buf.size() - dstream.avail_out;
    deflateEnd(&dstream);

    // inflate
    z_stream istream;
    memset(&istream, 0, sizeof(istream));
    ret = inflateInit(&istream);
    if (ret != Z_OK) return 7;

    istream.next_in = def_buf.data();
    istream.avail_in = static_cast<uInt>(def_size);

    std::vector<Bytef> infl_buf(orig_len);
    istream.next_out = infl_buf.data();
    istream.avail_out = static_cast<uInt>(infl_buf.size());

    ret = inflate(&istream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&istream);
        return 8;
    }
    size_t infl_size = infl_buf.size() - istream.avail_out;
    inflateEnd(&istream);

    if (infl_size != orig_len) return 9;
    if (memcmp(infl_buf.data(), original, orig_len) != 0) return 10;

    // 4) gzopen / gzwrite / gzread
    const char* fname = "test_zlib_api_sequence.gz";
    gzFile gz = gzopen(fname, "wb");
    if (!gz) return 11;
    int written = gzwrite(gz, original, static_cast<unsigned int>(orig_len));
    if (written == 0) { gzclose(gz); return 12; }
    gzclose(gz);

    gz = gzopen(fname, "rb");
    if (!gz) return 13;
    std::vector<char> gz_read_buf(orig_len + 1);
    int nread = gzread(gz, gz_read_buf.data(), static_cast<unsigned int>(orig_len));
    gzclose(gz);
    if (nread != static_cast<int>(orig_len)) {
        // clean up file
        std::remove(fname);
        return 14;
    }
    if (memcmp(gz_read_buf.data(), original, orig_len) != 0) {
        std::remove(fname);
        return 15;
    }
    // remove the temporary gz file
    std::remove(fname);

    // 5) verify crc32 and adler32 match on decompressed data
    uLong crc_check = crc32(0L, Z_NULL, 0);
    crc_check = crc32(crc_check, reinterpret_cast<const Bytef*>(infl_buf.data()), static_cast<uInt>(infl_size));
    if (crc_check != crc_orig) return 16;

    uLong adler_check = adler32(0L, Z_NULL, 0);
    adler_check = adler32(adler_check, reinterpret_cast<const Bytef*>(infl_buf.data()), static_cast<uInt>(infl_size));
    if (adler_check != adler_orig) return 17;

    // All tests passed
    return 66;
}