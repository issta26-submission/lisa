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
//<ID> 312
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and source preparation
    const char src_c[] = "zlib API sequence payload: prepare -> compress -> inflateCopy -> gz positions";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();

    // step 2: Compress source into a buffer
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Initialize an inflate stream and create a copy with inflateCopy
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompAlloc = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(z_stream));
    int rc_inf_copy = inflateCopy(&istrm_copy, &istrm);

    // step 4: Write compressed data to gz files, query positions, and cleanup
    gzFile gzf = gzopen("zlib_api_sequence_test.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, compBuf, (unsigned int)compLen);
    off_t tell_pos = gztell(gzf);
    off64_t offset64_main = gzoffset64(gzf);
    gzFile gzf_stdout = gzdopen(1, "wb");
    int rc_gzwrite2 = gzwrite(gzf_stdout, compBuf, (unsigned int)(compLen > 16 ? 16U : (unsigned int)compLen));
    off64_t offset64_stdout = gzoffset64(gzf_stdout);
    int rc_gzclose1 = gzclose(gzf);
    int rc_gzclose2 = gzclose(gzf_stdout);
    int rc_inf_end_copy = inflateEnd(&istrm_copy);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(decompBuf);

    (void)version;
    (void)rc_comp;
    (void)bound;
    (void)compLen;
    (void)rc_inf_init;
    (void)rc_inf_copy;
    (void)rc_gzwrite;
    (void)tell_pos;
    (void)offset64_main;
    (void)rc_gzwrite2;
    (void)offset64_stdout;
    (void)rc_gzclose1;
    (void)rc_gzclose2;
    (void)rc_inf_end_copy;
    (void)rc_inf_end;

    // API sequence test completed successfully
    return 66;
}