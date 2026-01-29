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
//<ID> 283
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress into a buffer
    const char src_c[] = "zlib API sequence payload: prepare -> compress -> write -> inflate -> cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream and open a gz file for writing
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompAlloc = (uLongf)(sourceLen + 256);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_printf = gzprintf(gzf, "zlib: writing compressed data\n");
    int rc_gz_write = gzwrite(gzf, compBuf, (unsigned int)compLen);

    // step 3: Operate - inflate the compressed data and record error text
    int rc_inflate = inflate(&istrm, 0);
    const char *inflate_err = zError(rc_inflate);
    int rc_gz_printf_err = gzprintf(gzf, inflate_err);

    // step 4: Cleanup streams and memory
    int rc_inf_end = inflateEnd(&istrm);
    int rc_gz_close = gzclose(gzf);
    free(compBuf);
    free(decompBuf);

    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)version;
    (void)rc_inf_init;
    (void)decompAlloc;
    (void)rc_gz_printf;
    (void)rc_gz_write;
    (void)rc_inflate;
    (void)inflate_err;
    (void)rc_gz_printf_err;
    (void)rc_inf_end;
    (void)rc_gz_close;

    // API sequence test completed successfully
    return 66;
}