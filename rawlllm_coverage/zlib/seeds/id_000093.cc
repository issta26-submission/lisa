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
//<ID> 93
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // include headers here so the function is self-contained when pasted into a file
    #include <zlib.h>
    #include <cstring>
    #include <vector>
    #include <cstdio>
    #include <cstdlib>

    // Sample input data (make it reasonably long so compression does something)
    const char *input = "The quick brown fox jumps over the lazy dog. "
                        "This is a sample string to exercise zlib APIs. "
                        "Repeat: The quick brown fox jumps over the lazy dog. "
                        "End of sample.";
    uLong srcLen = static_cast<uLong>(std::strlen(input));

    // 1) Simple one-shot compression using compress2() and uncompress()
    uLong bound = compressBound(srcLen);
    std::vector<unsigned char> compBuf(bound);
    uLongf compLen = static_cast<uLongf>(bound);

    int res = compress2(compBuf.data(), &compLen,
                        reinterpret_cast<const Bytef*>(input), srcLen,
                        Z_BEST_COMPRESSION);
    if (res != Z_OK) return -1;

    std::vector<unsigned char> decompBuf(srcLen + 1);
    uLongf decompLen = srcLen;
    res = uncompress(decompBuf.data(), &decompLen,
                     compBuf.data(), compLen);
    if (res != Z_OK) return -2;
    if (decompLen != srcLen) return -3;
    if (std::memcmp(decompBuf.data(), input, srcLen) != 0) return -4;

    // 2) Stream compression using deflateInit / deflate / deflateEnd
    z_stream dstream;
    std::memset(&dstream, 0, sizeof(dstream));
    res = deflateInit(&dstream, Z_DEFAULT_COMPRESSION);
    if (res != Z_OK) return -5;

    dstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    dstream.avail_in = static_cast<uInt>(srcLen); // safe for small data

    std::vector<unsigned char> compStreamBuf(compressBound(srcLen));
    dstream.next_out = compStreamBuf.data();
    dstream.avail_out = static_cast<uInt>(compStreamBuf.size());

    res = deflate(&dstream, Z_FINISH);
    if (res != Z_STREAM_END) { deflateEnd(&dstream); return -6; }
    uLong compStreamLen = dstream.total_out;
    deflateEnd(&dstream);

    // 3) Stream decompression using inflateInit / inflate / inflateEnd
    z_stream istream;
    std::memset(&istream, 0, sizeof(istream));
    res = inflateInit(&istream);
    if (res != Z_OK) return -7;

    istream.next_in = compStreamBuf.data();
    istream.avail_in = static_cast<uInt>(compStreamLen);

    std::vector<unsigned char> decompStreamBuf(srcLen + 1);
    istream.next_out = decompStreamBuf.data();
    istream.avail_out = static_cast<uInt>(decompStreamBuf.size());

    res = inflate(&istream, Z_FINISH);
    if (res != Z_STREAM_END) { inflateEnd(&istream); return -8; }
    uLong decompStreamLen = istream.total_out;
    inflateEnd(&istream);

    if (decompStreamLen != srcLen) return -9;
    if (std::memcmp(decompStreamBuf.data(), input, srcLen) != 0) return -10;

    // 4) gz* convenience API: gzopen, gzwrite, gzread, gzclose
    const char *tempFile = "test_zlib_api_sequence_temp.gz";
    gzFile gz = gzopen(tempFile, "wb");
    if (!gz) return -11;
    int written = gzwrite(gz, input, static_cast<unsigned int>(srcLen));
    if (written != static_cast<int>(srcLen)) { gzclose(gz); std::remove(tempFile); return -12; }
    gzclose(gz);

    gz = gzopen(tempFile, "rb");
    if (!gz) { std::remove(tempFile); return -13; }
    std::vector<char> gzReadBuf(srcLen + 16);
    int read = gzread(gz, gzReadBuf.data(), static_cast<unsigned int>(gzReadBuf.size()));
    gzclose(gz);
    if (read != static_cast<int>(srcLen)) { std::remove(tempFile); return -14; }
    if (std::memcmp(gzReadBuf.data(), input, srcLen) != 0) { std::remove(tempFile); return -15; }
    std::remove(tempFile);

    // 5) Adler32 and CRC32 utility APIs
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(input), srcLen);
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input), srcLen);
    // Basic sanity check: checksums should be non-zero for our sample
    if (adl == 0 || crc == 0) return -16;

    // All operations succeeded
    return 66;
}