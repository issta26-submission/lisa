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
//<ID> 158
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include necessary headers here so the function is self-contained when pasted into a file.
    // Preprocessor directives are allowed anywhere; they will be processed before compilation.
    #include <zlib.h>
    #include <cstring>
    #include <vector>
    #include <cstdio>
    #include <cstdlib>

    // Original data to compress/decompress
    const char* original = "The quick brown fox jumps over the lazy dog. "
                           "This is a test string for zlib API sequence validation.";
    const uLong srcLen = static_cast<uLong>(std::strlen(original));

    // 1) Compute checksums (crc32 and adler32)
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(original), srcLen);
    uLong ad = adler32(0L, Z_NULL, 0);
    ad = adler32(ad, reinterpret_cast<const Bytef*>(original), srcLen);

    // Basic sanity: checksums must be non-zero for non-empty input
    if (srcLen > 0 && (crc == 0 || ad == 0)) return 1;

    // 2) compress() / uncompress() (simple one-shot API)
    uLong compBound = compressBound(srcLen);
    std::vector<Bytef> compBuf(compBound);
    int ret = compress(compBuf.data(), &compBound,
                       reinterpret_cast<const Bytef*>(original), srcLen);
    if (ret != Z_OK) return 2; // compress failed

    // Uncompress into buffer of expected original size
    std::vector<Bytef> uncmpBuf(srcLen);
    uLong uncmpLen = srcLen;
    ret = uncompress(uncmpBuf.data(), &uncmpLen, compBuf.data(), compBound);
    if (ret != Z_OK) return 3; // uncompress failed
    if (uncmpLen != srcLen) return 4; // size mismatch after uncompress
    if (std::memcmp(uncmpBuf.data(), original, srcLen) != 0) return 5; // data mismatch

    // 3) deflate (stream API) -> inflate (stream API)
    // Prepare z_stream for deflate
    z_stream dstrm;
    std::memset(&dstrm, 0, sizeof(dstrm));
    ret = deflateInit(&dstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 6;

    dstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original));
    dstrm.avail_in = srcLen;

    // Output buffer sized using compressBound for safety
    std::vector<Bytef> defOut(compBound);
    dstrm.next_out = defOut.data();
    dstrm.avail_out = static_cast<uInt>(defOut.size());

    ret = deflate(&dstrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&dstrm);
        return 7; // deflate did not finish (unexpected)
    }
    uLong deflatedSize = static_cast<uLong>(defOut.size()) - dstrm.avail_out;
    deflateEnd(&dstrm);

    // Inflate back
    z_stream istrm;
    std::memset(&istrm, 0, sizeof(istrm));
    ret = inflateInit(&istrm);
    if (ret != Z_OK) return 8;

    istrm.next_in = defOut.data();
    istrm.avail_in = static_cast<uInt>(deflatedSize);

    std::vector<Bytef> inflOut(srcLen);
    istrm.next_out = inflOut.data();
    istrm.avail_out = static_cast<uInt>(inflOut.size());

    ret = inflate(&istrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&istrm);
        return 9; // inflate did not finish (unexpected)
    }
    uLong inflatedSize = static_cast<uLong>(inflOut.size()) - istrm.avail_out;
    inflateEnd(&istrm);

    if (inflatedSize != srcLen) return 10;
    if (std::memcmp(inflOut.data(), original, srcLen) != 0) return 11;

    // 4) gzopen / gzwrite / gzread / gzclose (file-based gz API)
    const char* tmpFilename = "test_zlib_api_sequence_tmp.gz";
    gzFile gz = gzopen(tmpFilename, "wb");
    if (!gz) return 12;
    int wrote = gzwrite(gz, original, static_cast<unsigned int>(srcLen));
    if (wrote != static_cast<int>(srcLen)) {
        gzclose(gz);
        std::remove(tmpFilename);
        return 13;
    }
    if (gzclose(gz) != 0) {
        std::remove(tmpFilename);
        return 14;
    }

    // Read back
    gz = gzopen(tmpFilename, "rb");
    if (!gz) {
        std::remove(tmpFilename);
        return 15;
    }
    std::vector<char> gzReadBuf(srcLen);
    int nread = gzread(gz, gzReadBuf.data(), static_cast<unsigned int>(srcLen));
    if (nread < 0) {
        gzclose(gz);
        std::remove(tmpFilename);
        return 16;
    }
    gzclose(gz);
    std::remove(tmpFilename);

    if (static_cast<uLong>(nread) != srcLen) return 17;
    if (std::memcmp(gzReadBuf.data(), original, srcLen) != 0) return 18;

    // If we reached this point, all zlib API sequences succeeded
    return 66;
}