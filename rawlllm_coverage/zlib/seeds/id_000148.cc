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
//<ID> 148
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // include headers here so this function is self-contained when pasted
    #include <zlib.h>
    #include <string>
    #include <vector>
    #include <cstring>
    #include <cstdio>

    // Sample data to compress/decompress
    const std::string input = 
        "The quick brown fox jumps over the lazy dog. "
        "This is a test of zlib APIs: deflate/inflate, compress/uncompress, crc32, adler32 and gz*.";
    const size_t input_size = input.size();

    // 1) Streaming compression with deflate / deflateEnd
    z_stream dstrm;
    std::memset(&dstrm, 0, sizeof(dstrm));
    int ret = deflateInit(&dstrm, Z_BEST_COMPRESSION);
    if (ret != Z_OK) return 1;

    // allocate output buffer using compressBound to ensure it's large enough
    uLong bound = compressBound(static_cast<uLong>(input_size));
    std::vector<Bytef> compBuf(static_cast<size_t>(bound));
    dstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    dstrm.avail_in = static_cast<uInt>(input_size);
    dstrm.next_out = compBuf.data();
    dstrm.avail_out = static_cast<uInt>(compBuf.size());

    // Perform compression; Z_FINISH should finish the stream in one call because buffer is large enough
    ret = deflate(&dstrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&dstrm);
        return 2;
    }
    uLong comp_size = static_cast<uLong>(compBuf.size()) - dstrm.avail_out;
    deflateEnd(&dstrm);

    // 2) Streaming decompression with inflate / inflateEnd
    z_stream istrm;
    std::memset(&istrm, 0, sizeof(istrm));
    ret = inflateInit(&istrm);
    if (ret != Z_OK) return 3;

    std::vector<Bytef> decompBuf(input_size);
    istrm.next_in = compBuf.data();
    istrm.avail_in = static_cast<uInt>(comp_size);
    istrm.next_out = decompBuf.data();
    istrm.avail_out = static_cast<uInt>(decompBuf.size());

    ret = inflate(&istrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&istrm);
        return 4;
    }
    uLong decomp_size = static_cast<uLong>(decompBuf.size()) - istrm.avail_out;
    inflateEnd(&istrm);

    if (decomp_size != input_size) return 5;
    if (std::memcmp(decompBuf.data(), input.data(), input_size) != 0) return 6;

    // 3) Convenience APIs: compress2 / uncompress
    uLong bound2 = compressBound(static_cast<uLong>(input_size));
    std::vector<Bytef> comp2Buf(static_cast<size_t>(bound2));
    uLong comp2_size = bound2;
    ret = compress2(comp2Buf.data(), &comp2_size,
                    reinterpret_cast<const Bytef*>(input.data()),
                    static_cast<uLong>(input_size),
                    Z_BEST_COMPRESSION);
    if (ret != Z_OK) return 7;

    std::vector<Bytef> uncompressBuf(input_size);
    uLong uncompress_size = static_cast<uLong>(input_size);
    ret = uncompress(uncompressBuf.data(), &uncompress_size,
                     comp2Buf.data(), comp2_size);
    if (ret != Z_OK) return 8;
    if (uncompress_size != input_size) return 9;
    if (std::memcmp(uncompressBuf.data(), input.data(), input_size) != 0) return 10;

    // 4) Checksum APIs: crc32 and adler32
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(input.data()), static_cast<uInt>(input_size));
    uLong adler_orig = adler32(0L, Z_NULL, 0);
    adler_orig = adler32(adler_orig, reinterpret_cast<const Bytef*>(input.data()), static_cast<uInt>(input_size));

    uLong crc_decomp = crc32(0L, Z_NULL, 0);
    crc_decomp = crc32(crc_decomp, decompBuf.data(), static_cast<uInt>(decomp_size));
    uLong adler_decomp = adler32(0L, Z_NULL, 0);
    adler_decomp = adler32(adler_decomp, decompBuf.data(), static_cast<uInt>(decomp_size));

    if (crc_orig != crc_decomp || adler_orig != adler_decomp) return 11;

    // 5) gz* convenience API: write gz file and read it back
    const char* gzname = "test_zlib_api_sequence_tmp.gz";
    gzFile gz = gzopen(gzname, "wb");
    if (!gz) return 12;
    int wrote = gzwrite(gz, input.data(), static_cast<unsigned int>(input_size));
    if (wrote != static_cast<int>(input_size)) {
        gzclose(gz);
        std::remove(gzname);
        return 13;
    }
    if (gzclose(gz) != Z_OK) {
        std::remove(gzname);
        return 14;
    }

    gz = gzopen(gzname, "rb");
    if (!gz) {
        std::remove(gzname);
        return 15;
    }
    std::vector<char> readBack(input_size);
    int read = gzread(gz, readBack.data(), static_cast<unsigned int>(input_size));
    if (read < 0) {
        gzclose(gz);
        std::remove(gzname);
        return 16;
    }
    gzclose(gz);
    std::remove(gzname);

    if (static_cast<size_t>(read) != input_size) return 17;
    if (std::memcmp(readBack.data(), input.data(), input_size) != 0) return 18;

    // If we reached here, all zlib API usages succeeded and verified
    return 66;
}