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
//<ID> 21
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function exercises a sequence of zlib APIs:
    // - compress2 / uncompress
    // - crc32 / adler32
    // - deflateInit / deflate / deflateEnd
    // - inflateInit / inflate / inflateEnd
    // - gzopen / gzwrite / gzread / gzclose
    // - zlibVersion
    //
    // On success returns 66, otherwise returns a negative (or non-66) error code.

    const char *sample = "The quick brown fox jumps over the lazy dog. Zlib API test string.";
    unsigned long sample_len = (unsigned long)strlen(sample);

    // 1) compress2
    uLongf compBound = compressBound(sample_len);
    unsigned char *compBuf = (unsigned char*)malloc(compBound);
    if (!compBuf) return -1;
    int ret = compress2(compBuf, &compBound, (const unsigned char*)sample, sample_len, Z_BEST_COMPRESSION);
    if (ret != Z_OK) { free(compBuf); return -2; }
    uLongf compSize = compBound;

    // 2) uncompress
    uLongf uncompressLen = sample_len + 16;
    unsigned char *uncompBuf = (unsigned char*)malloc(uncompressLen);
    if (!uncompBuf) { free(compBuf); return -3; }
    ret = uncompress(uncompBuf, &uncompressLen, compBuf, compSize);
    if (ret != Z_OK) { free(compBuf); free(uncompBuf); return -4; }
    if (uncompressLen != sample_len || memcmp(uncompBuf, sample, sample_len) != 0) { free(compBuf); free(uncompBuf); return -5; }

    // 3) checksums: crc32 and adler32
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, (const unsigned char*)sample, sample_len);
    uLong crc_uncomp = crc32(0L, Z_NULL, 0);
    crc_uncomp = crc32(crc_uncomp, uncompBuf, uncompressLen);
    if (crc_orig != crc_uncomp) { free(compBuf); free(uncompBuf); return -6; }

    uLong adler_orig = adler32(0L, Z_NULL, 0);
    adler_orig = adler32(adler_orig, (const unsigned char*)sample, sample_len);
    uLong adler_uncomp = adler32(0L, Z_NULL, 0);
    adler_uncomp = adler32(adler_uncomp, uncompBuf, uncompressLen);
    if (adler_orig != adler_uncomp) { free(compBuf); free(uncompBuf); return -7; }

    // 4) deflate (stream API)
    {
        z_stream dstrm;
        memset(&dstrm, 0, sizeof(dstrm));
        ret = deflateInit(&dstrm, Z_DEFAULT_COMPRESSION);
        if (ret != Z_OK) { free(compBuf); free(uncompBuf); return -8; }

        dstrm.next_in = (unsigned char*)sample;
        dstrm.avail_in = (uInt)sample_len; // safe because sample_len is small

        unsigned char outbuf[128];
        unsigned char *deflated = NULL;
        size_t deflated_len = 0;
        int flush;
        do {
            dstrm.next_out = outbuf;
            dstrm.avail_out = sizeof(outbuf);
            flush = (dstrm.avail_in == 0) ? Z_FINISH : Z_NO_FLUSH;
            ret = deflate(&dstrm, flush);
            if (ret == Z_STREAM_ERROR) { deflateEnd(&dstrm); free(compBuf); free(uncompBuf); free(deflated); return -9; }
            size_t have = sizeof(outbuf) - dstrm.avail_out;
            if (have) {
                unsigned char *tmp = (unsigned char*)realloc(deflated, deflated_len + have);
                if (!tmp) { deflateEnd(&dstrm); free(compBuf); free(uncompBuf); free(deflated); return -10; }
                deflated = tmp;
                memcpy(deflated + deflated_len, outbuf, have);
                deflated_len += have;
            }
        } while (ret != Z_STREAM_END);
        deflateEnd(&dstrm);

        // 5) inflate the deflated stream
        z_stream istrm;
        memset(&istrm, 0, sizeof(istrm));
        ret = inflateInit(&istrm);
        if (ret != Z_OK) { free(compBuf); free(uncompBuf); free(deflated); return -11; }

        istrm.next_in = deflated;
        istrm.avail_in = (uInt)deflated_len;
        unsigned char inflate_out[128];
        unsigned char *inflated = NULL;
        size_t inflated_len = 0;
        do {
            istrm.next_out = inflate_out;
            istrm.avail_out = sizeof(inflate_out);
            ret = inflate(&istrm, Z_NO_FLUSH);
            if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) { inflateEnd(&istrm); free(compBuf); free(uncompBuf); free(deflated); free(inflated); return -12; }
            size_t have = sizeof(inflate_out) - istrm.avail_out;
            if (have) {
                unsigned char *tmp = (unsigned char*)realloc(inflated, inflated_len + have);
                if (!tmp) { inflateEnd(&istrm); free(compBuf); free(uncompBuf); free(deflated); free(inflated); return -13; }
                inflated = tmp;
                memcpy(inflated + inflated_len, inflate_out, have);
                inflated_len += have;
            }
        } while (ret != Z_STREAM_END);
        inflateEnd(&istrm);

        if (inflated_len != sample_len || memcmp(inflated, sample, sample_len) != 0) {
            free(compBuf); free(uncompBuf); free(deflated); free(inflated);
            return -14;
        }

        free(deflated);
        free(inflated);
    }

    // 6) gz* API: write sample to a gzip file then read it back
    {
        const char *fname = "test_zlib_api_sequence_tmp.gz";
        gzFile gz = gzopen(fname, "wb");
        if (!gz) { free(compBuf); free(uncompBuf); return -15; }
        int wrote = gzwrite(gz, sample, (unsigned int)sample_len);
        gzclose(gz);
        if (wrote != (int)sample_len) { remove(fname); free(compBuf); free(uncompBuf); return -16; }

        gz = gzopen(fname, "rb");
        if (!gz) { remove(fname); free(compBuf); free(uncompBuf); return -17; }
        unsigned char *readbuf = (unsigned char*)malloc(sample_len + 16);
        if (!readbuf) { gzclose(gz); remove(fname); free(compBuf); free(uncompBuf); return -18; }
        int read = gzread(gz, readbuf, (int)(sample_len + 16));
        gzclose(gz);
        remove(fname);
        if (read != (int)sample_len || memcmp(readbuf, sample, sample_len) != 0) { free(readbuf); free(compBuf); free(uncompBuf); return -19; }
        free(readbuf);
    }

    // 7) zlibVersion check
    const char *ver = zlibVersion();
    if (!ver || ver[0] == '\0') { free(compBuf); free(uncompBuf); return -20; }

    // Cleanup
    free(compBuf);
    free(uncompBuf);

    // All zlib API checks passed
    return 66;
}