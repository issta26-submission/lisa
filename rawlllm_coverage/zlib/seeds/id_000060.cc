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
//<ID> 60
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    /* Include headers here so the function is self-contained when pasted into a
       translation unit. Preprocessor includes are allowed anywhere in the file. */
    #include <zlib.h>
    #include <cstring>
    #include <cstdlib>

    /* Input data to compress/decompress */
    const unsigned char input[] =
        "The quick brown fox jumps over the lazy dog. This is a test of the zlib "
        "API sequence, including compress/uncompress, deflate/inflate, and checksums.";
    uLong srcLen = (uLong)(sizeof(input) - 1); /* exclude terminating NUL */

    /* 1) Compute checksums */
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, input, srcLen);

    uLong adler = adler32(0L, Z_NULL, 0);
    adler = adler32(adler, input, srcLen);

    /* 2) Use compress2() and uncompress() */
    uLong bound = compressBound(srcLen);
    unsigned char* compBuf = (unsigned char*)malloc(bound);
    if (!compBuf) return -1;

    uLong compLen = bound;
    int res = compress2(compBuf, &compLen, input, srcLen, Z_BEST_COMPRESSION);
    if (res != Z_OK) {
        free(compBuf);
        return -2;
    }

    unsigned char* decompBuf = (unsigned char*)malloc(srcLen);
    if (!decompBuf) {
        free(compBuf);
        return -3;
    }
    uLong decompLen = srcLen;
    res = uncompress(decompBuf, &decompLen, compBuf, compLen);
    if (res != Z_OK || decompLen != srcLen || std::memcmp(decompBuf, input, srcLen) != 0) {
        free(compBuf);
        free(decompBuf);
        return -4;
    }

    /* 3) Use deflate() and inflate() (z_stream) */
    /* Prepare an output buffer large enough (use compressBound as starting point). */
    uLong defOutSize = compressBound(srcLen) + 128;
    unsigned char* defOut = (unsigned char*)malloc(defOutSize);
    if (!defOut) {
        free(compBuf);
        free(decompBuf);
        return -5;
    }

    z_stream dstrm;
    std::memset(&dstrm, 0, sizeof(dstrm));
    res = deflateInit(&dstrm, Z_DEFAULT_COMPRESSION);
    if (res != Z_OK) {
        free(compBuf);
        free(decompBuf);
        free(defOut);
        return -6;
    }

    dstrm.next_in = (Bytef*)input;
    dstrm.avail_in = srcLen;
    dstrm.next_out = defOut;
    dstrm.avail_out = (uInt)defOutSize;

    /* One call should be enough because buffer is large; loop defensively */
    for (;;) {
        res = deflate(&dstrm, Z_FINISH);
        if (res == Z_STREAM_END) break;
        if (res != Z_OK) {
            deflateEnd(&dstrm);
            free(compBuf);
            free(decompBuf);
            free(defOut);
            return -7;
        }
        /* If not finished but no output space, fail (we sized buffer generously) */
        if (dstrm.avail_out == 0) {
            deflateEnd(&dstrm);
            free(compBuf);
            free(decompBuf);
            free(defOut);
            return -8;
        }
    }
    uLong deflatedSize = defOutSize - dstrm.avail_out;
    deflateEnd(&dstrm);

    /* Inflate back */
    unsigned char* inflOut = (unsigned char*)malloc(srcLen);
    if (!inflOut) {
        free(compBuf);
        free(decompBuf);
        free(defOut);
        return -9;
    }

    z_stream istrm;
    std::memset(&istrm, 0, sizeof(istrm));
    res = inflateInit(&istrm);
    if (res != Z_OK) {
        free(compBuf);
        free(decompBuf);
        free(defOut);
        free(inflOut);
        return -10;
    }

    istrm.next_in = defOut;
    istrm.avail_in = (uInt)deflatedSize;
    istrm.next_out = inflOut;
    istrm.avail_out = (uInt)srcLen;

    for (;;) {
        res = inflate(&istrm, Z_NO_FLUSH);
        if (res == Z_STREAM_END) break;
        if (res != Z_OK) {
            inflateEnd(&istrm);
            free(compBuf);
            free(decompBuf);
            free(defOut);
            free(inflOut);
            return -11;
        }
        /* If output buffer exhausted prematurely, fail (we sized exactly) */
        if (istrm.avail_out == 0) {
            inflateEnd(&istrm);
            free(compBuf);
            free(decompBuf);
            free(defOut);
            free(inflOut);
            return -12;
        }
    }
    uLong inflatedSize = srcLen - istrm.avail_out;
    inflateEnd(&istrm);

    if (inflatedSize != srcLen || std::memcmp(inflOut, input, srcLen) != 0) {
        free(compBuf);
        free(decompBuf);
        free(defOut);
        free(inflOut);
        return -13;
    }

    /* Optionally verify checksums coincide with the recomputed checksums on recovered data */
    uLong crc_check = crc32(0L, Z_NULL, 0);
    crc_check = crc32(crc_check, inflOut, inflatedSize);
    uLong adler_check = adler32(0L, Z_NULL, 0);
    adler_check = adler32(adler_check, inflOut, inflatedSize);
    if (crc_check != crc || adler_check != adler) {
        free(compBuf);
        free(decompBuf);
        free(defOut);
        free(inflOut);
        return -14;
    }

    /* Clean up */
    free(compBuf);
    free(decompBuf);
    free(defOut);
    free(inflOut);

    /* Success: return 66 as requested */
    return 66;
}