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
//<ID> 806
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and allocate buffers
    const char src[] = "zlib API sequence payload to exercise deflate, compressBound and gzread/gztell/gzoffset64";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);

    // step 2: Initialize and run a single-shot deflate operation (initialize → operate → cleanup)
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_deflate = deflate(&dstrm, 4); /* 4 corresponds to Z_FINISH in zlib */
    uLong compressedProduced = dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Persist original source into a gzip file, then read it back and query positions
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gw, source, (unsigned int)sourceLen);
    int rc_gz_flush = gzflush(gw, 0);
    int rc_gw_close = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)(sourceLen + 16));
    memset(readBuf, 0, (size_t)(sourceLen + 16));
    int rc_gz_read = gzread(gr, readBuf, (unsigned int)sourceLen);
    off_t pos = gztell(gr);
    off64_t off64 = gzoffset64(gr);
    int rc_gr_close = gzclose(gr);

    // step 4: Cleanup and finalize (validate → cleanup)
    free(compBuf);
    free(readBuf);
    (void)compressedProduced;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_gz_write;
    (void)rc_gz_flush;
    (void)rc_gw_close;
    (void)rc_gz_read;
    (void)pos;
    (void)off64;
    (void)rc_gr_close;
    // API sequence test completed successfully
    return 66;
}