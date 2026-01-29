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
//<ID> 159
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Original data
    const char *orig = "The quick brown fox jumps over the lazy dog. "
                       "This is a test string repeated to make data a bit larger. "
                       "1234567890";
    // Repeat the string a few times to have non-trivial size for streaming APIs
    std::string src;
    for (int i = 0; i < 16; ++i) src += orig;
    size_t src_len = src.size();

    // 1) compressBound + compress2
    uLong bound = compressBound(static_cast<uLong>(src_len));
    std::vector<unsigned char> comp1(bound);
    uLongf comp1_len = bound;
    int zret = compress2(comp1.data(), &comp1_len,
                         reinterpret_cast<const Bytef*>(src.data()),
                         static_cast<uLong>(src_len),
                         Z_BEST_COMPRESSION);
    if (zret != Z_OK) return -1;

    // 2) uncompress and verify
    std::vector<unsigned char> decomp1(src_len);
    uLongf decomp1_len = static_cast<uLongf>(src_len);
    zret = uncompress(decomp1.data(), &decomp1_len,
                      comp1.data(), comp1_len);
    if (zret != Z_OK) return -2;
    if (decomp1_len != src_len) return -3;
    if (std::memcmp(decomp1.data(), src.data(), src_len) != 0) return -4;

    // 3) Streaming deflate (z_stream, deflateInit/deflate/deflateEnd)
    z_stream defstr;
    std::memset(&defstr, 0, sizeof(defstr));
    if (deflateInit(&defstr, Z_DEFAULT_COMPRESSION) != Z_OK) return -5;

    defstr.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(src.data()));
    defstr.avail_in = static_cast<uInt>(src_len);

    uLongf stream_bound = compressBound(static_cast<uLong>(src_len)); // safe upper bound
    std::vector<unsigned char> comp2(stream_bound);
    defstr.next_out = comp2.data();
    defstr.avail_out = static_cast<uInt>(stream_bound);

    int def_ret = deflate(&defstr, Z_FINISH);
    if (def_ret != Z_STREAM_END) { deflateEnd(&defstr); return -6; }
    uLong comp2_len = defstr.total_out;
    deflateEnd(&defstr);

    // 4) Streaming inflate (z_stream, inflateInit/inflate/inflateEnd)
    z_stream instr;
    std::memset(&instr, 0, sizeof(instr));
    if (inflateInit(&instr) != Z_OK) return -7;

    instr.next_in = comp2.data();
    instr.avail_in = static_cast<uInt>(comp2_len);

    std::vector<unsigned char> decomp2(src_len);
    instr.next_out = decomp2.data();
    instr.avail_out = static_cast<uInt>(src_len);

    int inf_ret = inflate(&instr, Z_FINISH);
    if (inf_ret != Z_STREAM_END) { inflateEnd(&instr); return -8; }
    uLong decomp2_len = instr.total_out;
    inflateEnd(&instr);

    if (decomp2_len != src_len) return -9;
    if (std::memcmp(decomp2.data(), src.data(), src_len) != 0) return -10;

    // 5) adler32 and crc32 checksums
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(src.data()), static_cast<uInt>(src_len));
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(src.data()), static_cast<uInt>(src_len));
    // Just ensure they are non-zero for this data (very likely). Not strict verification.
    if (adl == 0 || crc == 0) return -11;

    // 6) gz* API: write and read back from a temporary .gz file
    const char *gzname = "test_zlib_api_sequence_tmp.gz";
    gzFile gz = gzopen(gzname, "wb");
    if (!gz) return -12;
    int wrote = gzwrite(gz, src.data(), static_cast<unsigned int>(src_len));
    if (wrote == 0) { gzclose(gz); remove(gzname); return -13; }
    gzclose(gz);

    gz = gzopen(gzname, "rb");
    if (!gz) { remove(gzname); return -14; }
    std::vector<char> gz_read_buf(src_len);
    int read_bytes = gzread(gz, gz_read_buf.data(), static_cast<unsigned int>(src_len));
    if (read_bytes <= 0) { gzclose(gz); remove(gzname); return -15; }
    gzclose(gz);

    // Clean up temporary file
    remove(gzname);

    if (static_cast<size_t>(read_bytes) != src_len) return -16;
    if (std::memcmp(gz_read_buf.data(), src.data(), src_len) != 0) return -17;

    // If we reach here, all API usages succeeded. Return 66 as requested.
    return 66;
}