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
//<ID> 398
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare payload and compress it in-memory
    const char src[] = "zlib API sequence payload: prepare -> compress2 -> write gz -> inflate -> read gz -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    int rc_comp2 = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Open a gz file and write compressed data (use a temporary filename to avoid blocking stdin/stdout)
    const char *tmpname = "zlib_api_test_tmp.gz";
    gzFile gzw = gzopen(tmpname, "wb");
    int rc_gzwrite = gzwrite(gzw, comp, (unsigned int)compLen);
    int rc_gzclose_w = gzclose(gzw);

    // step 3: Initialize inflate stream, feed it the compressed data, and then end it
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    uLong outBound = sourceLen + 64;
    Bytef *outbuf = (Bytef *)malloc((size_t)outBound);
    memset(outbuf, 0, (size_t)outBound);
    strm.next_out = outbuf;
    strm.avail_out = (uInt)outBound;
    int rc_inflate = inflate(&strm, 0);
    int rc_inflate_end = inflateEnd(&strm);

    // step 4: Open the gz reader for the file we wrote and attempt to read, then cleanup resources
    gzFile gzr = gzopen(tmpname, "rb");
    unsigned int readSize = 128;
    voidp readbuf = malloc((size_t)readSize);
    memset(readbuf, 0, (size_t)readSize);
    int rc_gzread = gzread(gzr, readbuf, readSize);
    int rc_gzclose_r = gzclose(gzr);
    const char *err0 = zError(0);
    const char *err_inflate = zError(rc_inflate);

    free(comp);
    free(outbuf);
    free(readbuf);

    (void)version;
    (void)rc_comp2;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_gzread;
    (void)rc_gzclose_r;
    (void)err0;
    (void)err_inflate;
    (void)tmpname;
    // API sequence test completed successfully
    return 66;
}