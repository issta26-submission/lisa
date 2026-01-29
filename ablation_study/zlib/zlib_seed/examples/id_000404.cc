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
//<ID> 404
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it in one shot
    const char src[] = "zlib API sequence payload: compress -> inflateInit_ -> inflate -> inflateSyncPoint -> gzopen -> gzwrite -> gzputc -> gzclose";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_compress = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize inflate stream and prepare output buffer
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compLen;
    uLong outSize = sourceLen + 16;
    Bytef *outbuf = (Bytef *)malloc((size_t)outSize);
    memset(outbuf, 0, (size_t)outSize);
    strm.next_out = outbuf;
    strm.avail_out = (uInt)outSize;

    // step 3: Perform inflate, check sync point, and finish inflate
    int rc_inflate = inflate(&strm, 0);
    int rc_syncpoint = inflateSyncPoint(&strm);
    int rc_inf_end = inflateEnd(&strm);

    // step 4: Write the inflated data to a gz file and clean up
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gz, outbuf, (unsigned int)strm.total_out);
    int rc_gzputc = gzputc(gz, '\n');
    int rc_gzclose = gzclose(gz);
    free(compBuf);
    free(outbuf);
    (void)version;
    (void)rc_compress;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_syncpoint;
    (void)rc_inf_end;
    (void)rc_gzwrite;
    (void)rc_gzputc;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}