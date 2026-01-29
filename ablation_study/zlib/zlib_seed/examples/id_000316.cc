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
//<ID> 316
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress into memory
    const char src_c[] = "zlib API sequence payload: prepare -> compress -> inflate copy -> gz operations";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize an inflate stream and set input/output buffers, then copy the stream
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLongf decompAlloc = (uLongf)(sourceLen + 256);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(z_stream));
    int rc_inf_copy = inflateCopy(&istrm_copy, &istrm);

    // step 3: Open gz files, write compressed data, and query positions
    gzFile gzf = gzopen("zlib_api_sequence_test.gz", "wb");
    int rc_gzwrite1 = gzwrite(gzf, compBuf, (unsigned int)compLen);
    off_t tell_pos = gztell(gzf);
    off64_t offset64_main = gzoffset64(gzf);
    gzFile gzd = gzdopen(1, "wb");
    int rc_gzwrite2 = gzwrite(gzd, compBuf, (unsigned int)compLen);
    off64_t offset64_stdout = gzoffset64(gzd);

    // step 4: Cleanup inflate streams, gz files and buffers
    int rc_inf_end_copy = inflateEnd(&istrm_copy);
    int rc_inf_end = inflateEnd(&istrm);
    int rc_gzclose1 = gzclose(gzf);
    int rc_gzclose2 = gzclose(gzd);
    free(compBuf);
    free(decompBuf);

    (void)version;
    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_inf_init;
    (void)decompAlloc;
    (void)rc_inf_copy;
    (void)rc_gzwrite1;
    (void)tell_pos;
    (void)offset64_main;
    (void)rc_gzwrite2;
    (void)offset64_stdout;
    (void)rc_inf_end_copy;
    (void)rc_inf_end;
    (void)rc_gzclose1;
    (void)rc_gzclose2;

    // API sequence test completed successfully
    return 66;
}