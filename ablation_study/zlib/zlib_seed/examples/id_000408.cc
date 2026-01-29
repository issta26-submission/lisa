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
//<ID> 408
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare payload and compress it one-shot
    const char src[] = "zlib API sequence payload: compress -> inflateInit_ -> inflate -> inflateSyncPoint -> gzwrite/gzputc";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize inflate stream and attach compressed input
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    uLong outBound = sourceLen + 64;
    Bytef *outbuf = (Bytef *)malloc((size_t)outBound);
    memset(outbuf, 0, (size_t)outBound);
    strm.next_out = outbuf;
    strm.avail_out = (uInt)outBound;

    // step 3: Inflate, check sync point, finalize inflate
    int rc_inflate = inflate(&strm, 0);
    uLong produced = strm.total_out;
    int rc_sync = inflateSyncPoint(&strm);
    int rc_inf_end = inflateEnd(&strm);

    // step 4: Write inflated output to a gzip file using gzwrite and gzputc, then cleanup
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, outbuf, (unsigned int)produced);
    int rc_gzputc = gzputc(gzf, 'Z');
    int rc_gzclose = gzclose(gzf);
    free(comp);
    free(outbuf);
    (void)version;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)produced;
    (void)rc_sync;
    (void)rc_inf_end;
    (void)rc_gzwrite;
    (void)rc_gzputc;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}