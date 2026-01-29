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
//<ID> 317
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare source + compress
    const char src_c[] = "zlib API sequence payload: prepare -> compress -> init streams -> write/gz positions";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize and configure inflate streams, then copy one to another
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompAlloc = (uLongf)(sourceLen + 512);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(z_stream));
    int rc_inf_copy = inflateCopy(&istrm_copy, &istrm);

    // step 3: Operate: write compressed data to gz files and query positions with required APIs
    gzFile gz1 = gzopen("zlib_api_sequence_test.gz", "wb");
    gzFile gz2 = gzdopen(1, "wb");
    int rc_gzwrite1 = gzwrite(gz1, compBuf, (unsigned int)compLen);
    int rc_gzwrite2 = gzwrite(gz2, compBuf, (unsigned int)compLen);
    off_t tell_pos = gztell(gz1);
    off64_t offset64 = gzoffset64(gz2);

    // step 4: Cleanup and finalize
    int rc_inf_end1 = inflateEnd(&istrm);
    int rc_inf_end2 = inflateEnd(&istrm_copy);
    int rc_gzclose1 = gzclose(gz1);
    int rc_gzclose2 = gzclose(gz2);
    free(compBuf);
    free(decompBuf);

    (void)version;
    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_inf_init;
    (void)rc_inf_copy;
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