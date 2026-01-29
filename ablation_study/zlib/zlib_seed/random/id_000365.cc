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
//<ID> 365
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload, version, checksums
    const char payload[] = "zlib API sequence payload: compress2 -> crc32_z -> deflate -> gzclose usage";
    const Bytef *source = (const Bytef *)payload;
    uLong sourceLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLong crc = crc32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: One-shot compression using compress2
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Initialize a deflate stream and perform a single deflate call
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    uLong dOutBound = compressBound(sourceLen);
    Bytef *dout = (Bytef *)malloc((size_t)dOutBound);
    memset(dout, 0, (size_t)dOutBound);
    dstrm.next_out = dout;
    dstrm.avail_out = (uInt)dOutBound;
    int rc_deflate = deflate(&dstrm, 0);

    // step 4: Persist the one-shot compressed data to a gzip file and cleanup
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)compBuf, (unsigned int)compLen);
    int rc_gzclose = gzclose(gzf);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(dout);

    (void)version;
    (void)crc;
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_def_end;

    // API sequence test completed successfully
    return 66;
}