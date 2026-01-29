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
//<ID> 402
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress it in one shot
    const char src[] = "zlib API sequence payload: compress -> inflateInit_ -> inflateSyncPoint -> gzwrite/gzputc -> gzclose";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize an inflate stream and set buffers
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

    // step 3: Inflate, check sync point, and finish inflate
    int rc_inflate = inflate(&strm, 0);
    int rc_sync = inflateSyncPoint(&strm);
    uLong produced = strm.total_out;
    int rc_inf_end = inflateEnd(&strm);

    // step 4: Write the decompressed data via gz APIs, then cleanup
    gzFile gzf = gzdopen(1, "wb");
    int rc_gzwrite = gzwrite(gzf, outbuf, (unsigned int)produced);
    int rc_gzputc = gzputc(gzf, 'X');
    int rc_gzclose = gzclose(gzf);
    free(comp);
    free(outbuf);
    (void)source;
    (void)version;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_sync;
    (void)produced;
    (void)rc_inf_end;
    (void)rc_gzwrite;
    (void)rc_gzputc;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}