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
//<ID> 744
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and initialize deflate stream
    const char src[] = "zlib API sequence test payload for deflateInit2_, inflateReset, gzread and inflateEnd";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_deflate = deflate(&dstrm, 4);
    uLong compProduced = (uLong)dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 2: Persist compressed output to a gz file and close
    gzFile gf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf_w, compBuf, (unsigned int)compProduced);
    int rc_gzclose_w = gzclose(gf_w);

    // step 3: Re-open gz file for reading, read contents via gzread into buffer
    gzFile gf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    uLong readAlloc = sourceLen + 128;
    Bytef *readBuf = (Bytef *)malloc((size_t)readAlloc);
    memset(readBuf, 0, (size_t)readAlloc);
    int rc_gzread = gzread(gf_r, (voidp)readBuf, (unsigned int)readAlloc);
    int rc_gzclose_r = gzclose(gf_r);

    // step 4: Initialize inflate stream, reset it, then cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    int rc_inf_reset = inflateReset(&istrm);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(readBuf);
    (void)rc_def_init;
    (void)rc_deflate;
    (void)compProduced;
    (void)rc_def_end;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)rc_gzclose_r;
    (void)rc_inf_init;
    (void)rc_inf_reset;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}