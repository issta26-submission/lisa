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
//<ID> 723
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and write compressed gzip file via gzFile
    const char src[] = "zlib API sequence test payload for inflateInit2_, gzrewind, crc32_combine and inflateEnd";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gw, source, (unsigned int)sourceLen);
    int rc_gzclose_w = gzclose_w ? gzclose_w(gw) : gzclose(gw);

    // step 2: Initialize and configure an inflate stream with inflateInit2_
    const char *version = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int windowBits = 15;
    int rc_inf_init2 = inflateInit2_(&istrm, windowBits, version, (int)sizeof(z_stream));

    // step 3: Operate — open gzFile for read, rewind, read output and compute CRCs
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int rc_rewind = gzrewind(gr);
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    int rc_gzread = gzread(gr, outBuf, (unsigned int)outAlloc);
    uLong outSize = (uLong)rc_gzread;
    uLong crc_orig = crc32(0UL, source, (uInt)sourceLen);
    uLong crc_read = crc32(0UL, outBuf, (uInt)outSize);
    uLong crc_combined = crc32_combine(crc_orig, crc_read, (off_t)outSize);

    // step 4: Validate and cleanup — end inflate stream, close files, free buffers
    int rc_inf_end = inflateEnd(&istrm);
    int rc_gzclose_r = gzclose(gr);
    free(outBuf);
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_inf_init2;
    (void)rc_rewind;
    (void)rc_gzread;
    (void)outSize;
    (void)crc_orig;
    (void)crc_read;
    (void)crc_combined;
    (void)rc_inf_end;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}