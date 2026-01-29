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
//<ID> 399
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare payload and compress it one-shot
    const char src[] = "zlib API sequence payload: gzdopen -> gzwrite -> inflateInit_ -> inflate -> inflateEnd -> gzread -> zError";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp2 = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream and perform a single inflate call
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    uLong outBound = sourceLen;
    Bytef *outbuf = (Bytef *)malloc((size_t)outBound);
    memset(outbuf, 0, (size_t)outBound);
    strm.next_out = outbuf;
    strm.avail_out = (uInt)outBound;
    int rc_inflate = inflate(&strm, 0);
    int rc_inf_end = inflateEnd(&strm);

    // step 3: Open a gz stream on stdout via gzdopen, write the inflated data, attempt a read, then close
    gzFile gzf = gzdopen(1, "wb");
    int rc_gzwrite = gzwrite(gzf, outbuf, (unsigned int)strm.total_out);
    char readbuf[64];
    memset(readbuf, 0, sizeof(readbuf));
    int rc_gzread = gzread(gzf, readbuf, 16);
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup and query an informational error string
    const char *errstr = zError(0);
    free(comp);
    free(outbuf);
    (void)version;
    (void)rc_comp2;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)rc_gzwrite;
    (void)rc_gzread;
    (void)rc_gzclose;
    (void)errstr;
    // API sequence test completed successfully
    return 66;
}