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
//<ID> 120
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates several zlib API usages:
    // - checksum calculations (crc32, adler32)
    // - simple compression/uncompression (compress2 / uncompress)
    // - streaming compression/decompression (deflate/inflate)
    // - reuse of a deflate stream with deflateReset
    //
    // On success the function returns 66. On failure a negative value is returned.

    // Note: This function requires linking with zlib and inclusion of <zlib.h>
    // The test data sizes are modest so casting to uInt is safe here.

    using std::string;
    using std::vector;

    // Test input data: two different messages to show reuse of deflate stream.
    string src1;
    for (int i = 0; i < 50; ++i) src1 += "The quick brown fox jumps over the lazy dog. ";
    string src2;
    for (int i = 0; i < 30; ++i) src2 += "Lorem ipsum dolor sit amet, consectetur adipiscing elit. ";

    // 1) Compute checksums for original data
    uLong crc1 = crc32(0L, Z_NULL, 0);
    crc1 = crc32(crc1, reinterpret_cast<const Bytef*>(src1.data()), static_cast<uInt>(src1.size()));
    uLong adler1 = adler32(0L, Z_NULL, 0);
    adler1 = adler32(adler1, reinterpret_cast<const Bytef*>(src1.data()), static_cast<uInt>(src1.size()));

    uLong crc2 = crc32(0L, Z_NULL, 0);
    crc2 = crc32(crc2, reinterpret_cast<const Bytef*>(src2.data()), static_cast<uInt>(src2.size()));
    uLong adler2 = adler32(0L, Z_NULL, 0);
    adler2 = adler32(adler2, reinterpret_cast<const Bytef*>(src2.data()), static_cast<uInt>(src2.size()));

    // 2) Simple single-call compress/uncompress using compress2 / uncompress
    uLong bound1 = compressBound(static_cast<uLong>(src1.size()));
    vector<Bytef> comp1(bound1);
    uLongf comp1Len = static_cast<uLongf>(bound1);
    int res = compress2(comp1.data(), &comp1Len,
                        reinterpret_cast<const Bytef*>(src1.data()),
                        static_cast<uLong>(src1.size()),
                        Z_BEST_COMPRESSION);
    if (res != Z_OK) return -1;

    // Uncompress back
    vector<Bytef> decomp1(src1.size());
    uLongf decomp1Len = static_cast<uLongf>(src1.size());
    res = uncompress(decomp1.data(), &decomp1Len, comp1.data(), comp1Len);
    if (res != Z_OK) return -2;
    if (decomp1Len != src1.size()) return -3;
    if (memcmp(decomp1.data(), src1.data(), src1.size()) != 0) return -4;

    // 3) Streaming deflate (compress) with deflateInit / deflate / deflateReset / deflateEnd
    z_stream def;
    std::memset(&def, 0, sizeof(def));
    def.zalloc = Z_NULL;
    def.zfree = Z_NULL;
    def.opaque = Z_NULL;
    res = deflateInit(&def, Z_DEFAULT_COMPRESSION);
    if (res != Z_OK) return -5;

    // Compress src1 using deflate (streaming)
    uLong bound_stream1 = compressBound(static_cast<uLong>(src1.size()));
    vector<Bytef> comp_stream1(bound_stream1);
    def.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(src1.data()));
    def.avail_in = static_cast<uInt>(src1.size());
    def.next_out = comp_stream1.data();
    def.avail_out = static_cast<uInt>(comp_stream1.size());

    res = deflate(&def, Z_FINISH);
    if (res != Z_STREAM_END) {
        deflateEnd(&def);
        return -6;
    }
    uLong comp_stream1_len = def.total_out; // after Z_FINISH gives bytes produced
    // Prepare for next compression by resetting the deflate state
    res = deflateReset(&def);
    if (res != Z_OK) {
        deflateEnd(&def);
        return -7;
    }

    // Compress src2 using the same deflate stream after reset
    uLong bound_stream2 = compressBound(static_cast<uLong>(src2.size()));
    vector<Bytef> comp_stream2(bound_stream2);
    def.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(src2.data()));
    def.avail_in = static_cast<uInt>(src2.size());
    def.next_out = comp_stream2.data();
    def.avail_out = static_cast<uInt>(comp_stream2.size());

    res = deflate(&def, Z_FINISH);
    if (res != Z_STREAM_END) {
        deflateEnd(&def);
        return -8;
    }
    uLong comp_stream2_len = def.total_out; // total_out resets to 0 after deflateReset, so now holds bytes for src2
    // Finish using deflate
    deflateEnd(&def);

    // 4) Streaming inflate (decompress) for both compressed blocks
    // Decompress comp_stream1
    z_stream inf;
    std::memset(&inf, 0, sizeof(inf));
    inf.zalloc = Z_NULL;
    inf.zfree = Z_NULL;
    inf.opaque = Z_NULL;
    res = inflateInit(&inf);
    if (res != Z_OK) return -9;

    vector<Bytef> out1(src1.size());
    inf.next_in = comp_stream1.data();
    inf.avail_in = static_cast<uInt>(comp_stream1_len);
    inf.next_out = out1.data();
    inf.avail_out = static_cast<uInt>(out1.size());

    res = inflate(&inf, Z_FINISH);
    if (res != Z_STREAM_END) {
        inflateEnd(&inf);
        return -10;
    }
    uLong out1_len = inf.total_out;
    if (out1_len != src1.size()) {
        inflateEnd(&inf);
        return -11;
    }
    if (memcmp(out1.data(), src1.data(), src1.size()) != 0) {
        inflateEnd(&inf);
        return -12;
    }
    inflateEnd(&inf);

    // Decompress comp_stream2
    std::memset(&inf, 0, sizeof(inf));
    inf.zalloc = Z_NULL;
    inf.zfree = Z_NULL;
    inf.opaque = Z_NULL;
    res = inflateInit(&inf);
    if (res != Z_OK) return -13;

    vector<Bytef> out2(src2.size());
    inf.next_in = comp_stream2.data();
    inf.avail_in = static_cast<uInt>(comp_stream2_len);
    inf.next_out = out2.data();
    inf.avail_out = static_cast<uInt>(out2.size());

    res = inflate(&inf, Z_FINISH);
    if (res != Z_STREAM_END) {
        inflateEnd(&inf);
        return -14;
    }
    uLong out2_len = inf.total_out;
    if (out2_len != src2.size()) {
        inflateEnd(&inf);
        return -15;
    }
    if (memcmp(out2.data(), src2.data(), src2.size()) != 0) {
        inflateEnd(&inf);
        return -16;
    }
    inflateEnd(&inf);

    // 5) Verify that checksums are consistent for the decompressed outputs
    uLong crc1_check = crc32(0L, Z_NULL, 0);
    crc1_check = crc32(crc1_check, reinterpret_cast<const Bytef*>(out1.data()), static_cast<uInt>(out1_len));
    if (crc1_check != crc1) return -17;

    uLong adler1_check = adler32(0L, Z_NULL, 0);
    adler1_check = adler32(adler1_check, reinterpret_cast<const Bytef*>(out1.data()), static_cast<uInt>(out1_len));
    if (adler1_check != adler1) return -18;

    uLong crc2_check = crc32(0L, Z_NULL, 0);
    crc2_check = crc32(crc2_check, reinterpret_cast<const Bytef*>(out2.data()), static_cast<uInt>(out2_len));
    if (crc2_check != crc2) return -19;

    uLong adler2_check = adler32(0L, Z_NULL, 0);
    adler2_check = adler32(adler2_check, reinterpret_cast<const Bytef*>(out2.data()), static_cast<uInt>(out2_len));
    if (adler2_check != adler2) return -20;

    // All tests passed
    return 66;
}