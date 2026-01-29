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
//<ID> 318
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

    // step 2: Compress source into a buffer to provide input for inflate streams
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Initialize inflate source, copy it into a destination stream, and work with gz files
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

    z_stream dest;
    memset(&dest, 0, sizeof(z_stream));
    int rc_inf_copy = inflateCopy(&dest, &istrm);

    gzFile gzf = gzopen("zlib_api_sequence_test.gz", "wb");
    gzFile gzd = gzdopen(1, "wb");
    int rc_gzwrite1 = 0;
    int rc_gzwrite2 = 0;
    if (gzf) rc_gzwrite1 = gzwrite(gzf, source, (unsigned int)sourceLen);
    if (gzd) rc_gzwrite2 = gzwrite(gzd, source, (unsigned int)sourceLen);
    off_t tell_pos_gzf = gztell(gzf);
    off_t tell_pos_gzd = gztell(gzd);
    off64_t offset64_gzf = gzoffset64(gzf);

    // step 4: Cleanup streams, files and buffers
    int rc_inf_end_src = inflateEnd(&istrm);
    int rc_inf_end_dest = inflateEnd(&dest);
    int rc_gzclose1 = gzclose(gzf);
    int rc_gzclose2 = gzclose(gzd);
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
    (void)tell_pos_gzf;
    (void)tell_pos_gzd;
    (void)offset64_gzf;
    (void)rc_inf_end_src;
    (void)rc_inf_end_dest;
    (void)rc_gzclose1;
    (void)rc_gzclose2;

    // API sequence test completed successfully
    return 66;
}