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
//<ID> 473
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and perform a deflate (compress) pass
    const char src[] = "zlib API sequence payload: initialize -> deflate -> query pending -> inflate -> gz IO -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_deflate = deflate(&dstrm, 0);
    unsigned int pending_bits = 0;
    int pending_count = 0;
    int rc_pending = deflatePending(&dstrm, &pending_bits, &pending_count);

    // step 2: Initialize inflate stream and perform inflation (decompress)
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)dstrm.total_out;
    uLong outCap = sourceLen * 2 + 16;
    Bytef *outBuf = (Bytef *)malloc((size_t)outCap);
    memset(outBuf, 0, (size_t)outCap);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outCap;
    int rc_inflate = inflate(&istrm, 0);
    int rc_inf_end = inflateEnd(&istrm);

    // step 3: Use gzFile APIs to write decompressed data and query direct access
    gzFile gf = gzopen("zlib_sequence_output.gz", "wb");
    z_size_t written = 0;
    written = gzfwrite((voidpc)outBuf, (z_size_t)1, (z_size_t)istrm.total_out, gf);
    int rc_gzdirect = gzdirect(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup deflate stream and allocated memory
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(outBuf);
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_pending;
    (void)pending_bits;
    (void)pending_count;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)written;
    (void)rc_gzdirect;
    (void)rc_gzclose;
    (void)rc_def_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}