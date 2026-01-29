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
//<ID> 310
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare compressed payload
    const char src_c[] = "zlib API sequence payload: prepare -> compress -> inflateCopy -> gz positions";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize an inflate stream and create a copy via inflateCopy
    z_stream istrm_src;
    memset(&istrm_src, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm_src, version, (int)sizeof(z_stream));
    istrm_src.next_in = compBuf;
    istrm_src.avail_in = (uInt)compLen;
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(z_stream));
    int rc_inf_copy = inflateCopy(&istrm_copy, &istrm_src);

    // step 3: Write compressed buffer to a gzip file and query positions using gzopen/gzdopen/gztell/gzoffset64
    gzFile gzf = gzopen("zlib_api_sequence_test.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, compBuf, (unsigned int)compLen);
    off_t tell_pos = gztell(gzf);
    gzFile gzf_fd = gzdopen(1, "wb");
    off64_t offset64 = gzoffset64(gzf_fd);

    // step 4: Cleanup inflate streams, gz files and buffers
    int rc_inf_end_src = inflateEnd(&istrm_src);
    int rc_inf_end_copy = inflateEnd(&istrm_copy);
    int rc_gzclose1 = gzclose(gzf);
    int rc_gzclose2 = gzclose(gzf_fd);
    free(compBuf);

    (void)version;
    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_inf_init;
    (void)rc_inf_copy;
    (void)rc_gzwrite;
    (void)tell_pos;
    (void)offset64;
    (void)rc_inf_end_src;
    (void)rc_inf_end_copy;
    (void)rc_gzclose1;
    (void)rc_gzclose2;

    // API sequence test completed successfully
    return 66;
}