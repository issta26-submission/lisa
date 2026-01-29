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
//<ID> 201
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here so the function is self-contained when inserted into a file.
    // (Preprocessor includes are allowed inside a function.)
    #include <zlib.h>
    #include <vector>
    #include <cstring>
    #include <cstdio>
    #include <cstdlib>

    const char* input = "The quick brown fox jumps over the lazy dog";
    uLong inputLen = static_cast<uLong>(std::strlen(input));

    // 1) adler32 and crc32
    uLong adler = adler32(0L, Z_NULL, 0);
    (void)adler; // base value (should be 1), but compute full checksum below

    uLong adler_full = adler32(0L, reinterpret_cast<const Bytef*>(input), inputLen);
    uLong crc_full = crc32(0L, reinterpret_cast<const Bytef*>(input), inputLen);

    // Basic sanity: checksums should be non-zero for non-empty input
    if (inputLen > 0 && (adler_full == 0 || crc_full == 0)) return 1;

    // 2) compress() / uncompress() one-shot API
    uLong bound = compressBound(inputLen);
    std::vector<Bytef> compbuf(bound);
    uLongf compSize = bound;

    int cmp_ret = compress(compbuf.data(), &compSize,
                           reinterpret_cast<const Bytef*>(input), inputLen);
    if (cmp_ret != Z_OK) return 2;

    std::vector<Bytef> decompbuf(inputLen + 1);
    uLongf decompSize = inputLen;
    int ucmp_ret = uncompress(decompbuf.data(), &decompSize,
                              compbuf.data(), compSize);
    if (ucmp_ret != Z_OK) return 3;
    if (decompSize != inputLen) return 4;
    if (std::memcmp(decompbuf.data(), input, inputLen) != 0) return 5;

    // 3) deflate/inflate streaming APIs
    z_stream dstream;
    std::memset(&dstream, 0, sizeof(dstream));
    if (deflateInit(&dstream, Z_DEFAULT_COMPRESSION) != Z_OK) return 6;
    dstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    dstream.avail_in = inputLen;

    // allocate output using compressBound
    std::vector<Bytef> streamComp(compSize);
    dstream.next_out = streamComp.data();
    dstream.avail_out = static_cast<uInt>(streamComp.size());

    int def_ret = deflate(&dstream, Z_FINISH);
    if (def_ret != Z_STREAM_END && def_ret != Z_OK) {
        deflateEnd(&dstream);
        return 7;
    }
    uLong streamCompSize = streamComp.size() - dstream.avail_out;
    if (deflateEnd(&dstream) != Z_OK) return 8;

    // inflate the streamed compressed data
    z_stream istream;
    std::memset(&istream, 0, sizeof(istream));
    if (inflateInit(&istream) != Z_OK) return 9;
    istream.next_in = streamComp.data();
    istream.avail_in = static_cast<uInt>(streamCompSize);
    std::vector<Bytef> streamDecomp(inputLen + 1);
    istream.next_out = streamDecomp.data();
    istream.avail_out = static_cast<uInt>(streamDecomp.size());

    int inf_ret = inflate(&istream, Z_FINISH);
    if (inf_ret != Z_STREAM_END && inf_ret != Z_OK) {
        inflateEnd(&istream);
        return 10;
    }
    uLong streamDecompSize = streamDecomp.size() - istream.avail_out;
    if (inflateEnd(&istream) != Z_OK) return 11;
    if (streamDecompSize != inputLen) return 12;
    if (std::memcmp(streamDecomp.data(), input, inputLen) != 0) return 13;

    // 4) gz* API: write and read a gz file
    const char* fname = "test_zlib_api_sequence.gz";
    gzFile gz = gzopen(fname, "wb");
    if (!gz) return 14;
    int wrote = gzwrite(gz, reinterpret_cast<const void*>(input), static_cast<unsigned int>(inputLen));
    if (wrote != static_cast<int>(inputLen)) {
        gzclose(gz);
        std::remove(fname);
        return 15;
    }
    if (gzclose(gz) != Z_OK) {
        std::remove(fname);
        return 16;
    }

    // read it back
    gzFile gz2 = gzopen(fname, "rb");
    if (!gz2) {
        std::remove(fname);
        return 17;
    }
    std::vector<char> gzreadbuf(inputLen + 1);
    int nread = gzread(gz2, gzreadbuf.data(), static_cast<unsigned int>(inputLen));
    if (nread < 0) { // gzread returns -1 on error
        gzclose(gz2);
        std::remove(fname);
        return 18;
    }
    if (gzclose(gz2) != Z_OK) {
        std::remove(fname);
        return 19;
    }
    if (static_cast<uLong>(nread) != inputLen) {
        std::remove(fname);
        return 20;
    }
    if (std::memcmp(gzreadbuf.data(), input, inputLen) != 0) {
        std::remove(fname);
        return 21;
    }

    // cleanup file
    std::remove(fname);

    // If we've reached this point, all tested zlib APIs succeeded.
    (void)adler_full;
    (void)crc_full;
    return 66;
}