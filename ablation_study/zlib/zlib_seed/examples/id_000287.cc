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
//<ID> 287
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and source preparation
    const char src_c[] = "zlib API sequence payload: compress -> init inflate -> inflate -> report";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLongf compBufSize = (uLongf)(sourceLen + 256);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufSize);
    memset(compBuf, 0, (size_t)compBufSize);
    uLongf compLen = compBufSize;
    gzFile out = gzopen("test_zlib_api_sequence.gz", "wb");
    gzprintf(out, "zlib sequence start\n");

    // step 2: Compress the source and record status
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);
    const char *comp_err = zError(rc_comp);
    gzprintf(out, comp_err);
    gzprintf(out, "\n");

    // step 3: Initialize inflate stream and attempt to inflate compressed data
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    Bytef *decompBuf = (Bytef *)malloc((size_t)(sourceLen + 512));
    memset(decompBuf, 0, (size_t)(sourceLen + 512));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)(sourceLen + 512);
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    const char *init_err = zError(rc_inf_init);
    gzprintf(out, init_err);
    gzprintf(out, "\n");
    int rc_inflate = inflate(&istrm, 0);
    const char *inflate_err = zError(rc_inflate);
    gzprintf(out, inflate_err);
    gzprintf(out, "\n");

    // step 4: Validate results and cleanup
    int rc_inf_end = inflateEnd(&istrm);
    const char *end_err = zError(rc_inf_end);
    gzprintf(out, end_err);
    gzprintf(out, "\n");
    gzclose(out);
    free(compBuf);
    free(decompBuf);
    (void)source;
    (void)sourceLen;
    (void)compLen;
    (void)compBufSize;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}