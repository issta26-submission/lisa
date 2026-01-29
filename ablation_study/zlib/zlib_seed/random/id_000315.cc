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
//<ID> 315
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char src_c[] = "zlib API sequence payload: prepare -> compress -> init inflate -> copy -> write gz streams";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream and create a copy with inflateCopy
    z_stream istrm;
    z_stream istrm_copy;
    memset(&istrm, 0, sizeof(z_stream));
    memset(&istrm_copy, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLongf decompAlloc = (uLongf)(sourceLen + 512);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    int rc_inf_copy = inflateCopy(&istrm_copy, &istrm);

    // step 3: Open gz files (gzopen and gzdopen), write compressed data and query positions
    gzFile gzf1 = gzopen("zlib_api_sequence_test.gz", "wb");
    int rc_gzwrite1 = gzwrite(gzf1, compBuf, (unsigned int)compLen);
    off_t tell_pos = gztell(gzf1);
    gzFile gzf2 = gzdopen(1, "wb");
    int rc_gzwrite2 = gzwrite(gzf2, compBuf, (unsigned int)compLen);
    off64_t offset64 = gzoffset64(gzf2);
    int rc_gzclose1 = gzclose(gzf1);
    int rc_gzclose2 = gzclose(gzf2);

    // step 4: Cleanup and finalize
    int rc_inf_end_orig = inflateEnd(&istrm);
    int rc_inf_end_copy = inflateEnd(&istrm_copy);
    free(compBuf);
    free(decompBuf);

    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inf_copy;
    (void)rc_gzwrite1;
    (void)tell_pos;
    (void)rc_gzwrite2;
    (void)offset64;
    (void)rc_gzclose1;
    (void)rc_gzclose2;
    (void)rc_inf_end_orig;
    (void)rc_inf_end_copy;
    (void)version;
    (void)compBound;
    (void)sourceLen;
    (void)source;
    (void)gzf1;
    (void)gzf2;

    // API sequence test completed successfully
    return 66;
}