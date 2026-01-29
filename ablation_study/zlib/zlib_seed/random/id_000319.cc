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
//<ID> 319
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare source + compress
    const char src_c[] = "zlib API sequence payload: prepare -> compress -> inflate copy -> gz operations";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream and create a copy
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompAlloc = (uLongf)(sourceLen + 512);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(z_stream));
    int rc_infl_copy = inflateCopy(&istrm_copy, &istrm);

    // step 3: Perform inflate on original and copied stream, then open gz files and query positions
    int rc_inflate_orig = inflate(&istrm, 0);
    int rc_inflate_copy = inflate(&istrm_copy, 0);
    gzFile gzf = gzopen("zlib_api_sequence_test.gz", "wb");
    gzFile gzfd = gzdopen(1, "wb");
    int rc_gzwrite1 = gzwrite(gzf, compBuf, (unsigned int)compLen);
    int rc_gzwrite2 = gzwrite(gzfd, compBuf, (unsigned int)compLen);
    off_t tell_pos = gztell(gzf);
    off64_t offset64 = gzoffset64(gzf);

    // step 4: Cleanup streams, gz files and buffers
    int rc_inf_end1 = inflateEnd(&istrm);
    int rc_inf_end2 = inflateEnd(&istrm_copy);
    int rc_gzclose1 = gzclose(gzf);
    int rc_gzclose2 = gzclose(gzfd);
    free(compBuf);
    free(decompBuf);

    (void)version;
    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_inf_init;
    (void)rc_infl_copy;
    (void)rc_inflate_orig;
    (void)rc_inflate_copy;
    (void)rc_gzwrite1;
    (void)rc_gzwrite2;
    (void)tell_pos;
    (void)offset64;
    (void)rc_inf_end1;
    (void)rc_inf_end2;
    (void)rc_gzclose1;
    (void)rc_gzclose2;

    // API sequence test completed successfully
    return 66;
}