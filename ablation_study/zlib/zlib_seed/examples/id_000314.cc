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
//<ID> 314
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare compressed data
    const char src_c[] = "zlib API sequence payload: prepare -> compress -> inflate copy -> gz positions";
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
    uLong decompAlloc = sourceLen + 512;
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    z_stream copyStrm;
    memset(&copyStrm, 0, sizeof(z_stream));
    int rc_copy = inflateCopy(&copyStrm, &istrm);

    // step 3: Open gz files, write data and query positions
    gzFile gzf_file = gzopen("zlib_api_sequence_test.gz", "wb");
    gzFile gzf_stdout = gzdopen(1, "wb");
    int rc_gzwrite_file = 0;
    int rc_gzwrite_stdout = 0;
    rc_gzwrite_file = gzwrite(gzf_file, compBuf, (unsigned int)compLen);
    rc_gzwrite_stdout = gzwrite(gzf_stdout, decompBuf, (unsigned int)decompAlloc);
    off_t tell_pos_file = gztell(gzf_file);
    off64_t offset64_stdout = gzoffset64(gzf_stdout);

    // step 4: Cleanup
    int rc_inf_end_orig = inflateEnd(&istrm);
    int rc_inf_end_copy = inflateEnd(&copyStrm);
    int rc_gzclose_file = gzclose(gzf_file);
    int rc_gzclose_stdout = gzclose(gzf_stdout);
    free(compBuf);
    free(decompBuf);

    (void)version;
    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_inf_init;
    (void)rc_copy;
    (void)rc_gzwrite_file;
    (void)rc_gzwrite_stdout;
    (void)tell_pos_file;
    (void)offset64_stdout;
    (void)rc_inf_end_orig;
    (void)rc_inf_end_copy;
    (void)rc_gzclose_file;
    (void)rc_gzclose_stdout;

    // API sequence test completed successfully
    return 66;
}