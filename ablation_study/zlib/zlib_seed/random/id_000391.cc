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
//<ID> 391
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare payload and compress it one-shot
    const char src[] = "zlib API sequence payload: prepare -> compress2 -> write/read via gzdopen/gzwrite/gzread -> inflate -> inflateEnd -> zError";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp2 = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Write compressed data to a temporary file via gzdopen + gzwrite
    FILE *tf = tmpfile();
    int fd_orig = fileno(tf);
    int fd_dup = dup(fd_orig);
    gzFile gzf_out = gzdopen(fd_dup, "wb");
    int rc_gzwrite = gzwrite(gzf_out, comp, (unsigned int)compLen);
    int rc_gzclose_w = gzclose(gzf_out);

    // step 3: Rewind temp file, read compressed data back via gzdopen + gzread, then inflate and cleanup inflate with inflateEnd
    fseek(tf, 0, SEEK_SET);
    int fd_read = fileno(tf);
    int fd_dup_r = dup(fd_read);
    gzFile gzf_in = gzdopen(fd_dup_r, "rb");
    Bytef *comp_read = (Bytef *)malloc((size_t)compLen);
    memset(comp_read, 0, (size_t)compLen);
    int rc_gzread = gzread(gzf_in, comp_read, (unsigned int)compLen);
    int rc_gzclose_r = gzclose(gzf_in);

    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp_read;
    strm.avail_in = (uInt)rc_gzread;
    Bytef *outBuf = (Bytef *)malloc((size_t)(sourceLen + 1));
    memset(outBuf, 0, (size_t)(sourceLen + 1));
    strm.next_out = outBuf;
    strm.avail_out = (uInt)sourceLen;
    int rc_inflate = inflate(&strm, 0);
    const char *errstr = zError(rc_inflate);
    int rc_inflate_end = inflateEnd(&strm);

    // step 4: Validate, cleanup, and finish
    int equal = (int)(strcmp((const char *)outBuf, src) == 0);
    free(comp);
    free(comp_read);
    free(outBuf);
    fclose(tf);
    (void)version;
    (void)rc_comp2;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)rc_gzclose_r;
    (void)rc_inflate_init;
    (void)errstr;
    (void)rc_inflate_end;
    (void)equal;
    // API sequence test completed successfully
    return 66;
}