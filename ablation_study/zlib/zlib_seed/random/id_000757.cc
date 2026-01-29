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
//<ID> 757
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and buffers
    const char src[] = "zlib API sequence payload for deflateInit2_, gzopen64, gzsetparams, inflateInit2_ and deflateEnd";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);

    // step 2: Initialize and configure deflate stream via deflateInit2_
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;

    // step 3: Perform compression, persist original data via gz, and tune gz parameters
    int rc_deflate = deflate(&dstrm, 4); // Z_FINISH == 4
    uLong compProduced = (uLong)dstrm.total_out;
    gzFile gf = gzopen64("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_set = gzsetparams(gf, 9, 0);
    int rc_gz_write = gzwrite(gf, source, (unsigned int)sourceLen);
    int rc_gz_close = gzclose(gf);

    // step 4: Initialize inflate stream, decompress produced data, cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compProduced;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate = inflate(&istrm, 0);
    uLong inflatedSize = (uLong)istrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)rc_def_init;
    (void)rc_deflate;
    (void)compProduced;
    (void)rc_gz_set;
    (void)rc_gz_write;
    (void)rc_gz_close;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)inflatedSize;
    (void)rc_def_end;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}