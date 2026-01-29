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
//<ID> 390
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare payload and one-shot compress
    const char src[] = "zlib API sequence payload: compress2 -> gzdopen/gzwrite -> inflateInit_/inflate -> gzread -> inflateEnd -> zError";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Open a non-blocking sink and write compressed data (use /dev/null to avoid blocking on stdout)
    gzFile gz_w = gzopen("/dev/null", "wb");
    int rc_gzwrite = gzwrite(gz_w, comp, (unsigned int)compLen);
    int rc_gzclose_w = gzclose(gz_w);

    // step 3: Inflate the in-memory compressed data
    z_stream inf;
    memset(&inf, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = comp;
    inf.avail_in = (uInt)compLen;
    Bytef *out = (Bytef *)malloc((size_t)(sourceLen + 16));
    memset(out, 0, (size_t)(sourceLen + 16));
    inf.next_out = out;
    inf.avail_out = (uInt)(sourceLen + 16);
    int rc_inflate = inflate(&inf, 0);
    int rc_inflate_end = inflateEnd(&inf);

    // step 4: Demonstrate gzread on a non-blocking source and obtain a zlib error string
    gzFile gz_r = gzopen("/dev/null", "rb");
    unsigned int read_size = 64;
    voidp readbuf = malloc((size_t)read_size);
    memset(readbuf, 0, (size_t)read_size);
    int rc_gzread = gzread(gz_r, readbuf, read_size);
    const char *errstr = zError(0);
    int rc_gzclose_r = gzclose(gz_r);

    free(comp);
    free(out);
    free(readbuf);

    (void)version;
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_gzread;
    (void)errstr;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}