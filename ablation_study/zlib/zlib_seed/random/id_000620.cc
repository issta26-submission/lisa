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
//<ID> 620
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char src[] = "zlib API sequence test payload";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uInt compBufSize = (uInt)(sourceLen + 128);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufSize);
    memset(compBuf, 0, (size_t)compBufSize);
    uLongf compLen = (uLongf)compBufSize;

    // step 2: Compress the source into compBuf
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 3: Initialize a deflate stream and create a copy
    z_stream dsrc;
    z_stream ddst;
    memset(&dsrc, 0, sizeof(z_stream));
    memset(&ddst, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dsrc, 6, version, (int)sizeof(z_stream));
    dsrc.next_in = compBuf;
    dsrc.avail_in = (uInt)compLen;
    uInt workBufSize = compBufSize;
    Bytef *workBuf = (Bytef *)malloc((size_t)workBufSize);
    memset(workBuf, 0, (size_t)workBufSize);
    dsrc.next_out = workBuf;
    dsrc.avail_out = workBufSize;
    int rc_defcopy = deflateCopy(&ddst, &dsrc);

    // step 4: Initialize inflate, write compressed data to gzFile, finalize and cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)compLen);
    int rc_gzeof = gzeof(gf);
    int rc_gzclose = gzclose(gf);
    int rc_def_end1 = deflateEnd(&dsrc);
    int rc_def_end2 = deflateEnd(&ddst);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(workBuf);
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_defcopy;
    (void)rc_inf_init;
    (void)rc_gzwrite;
    (void)rc_gzeof;
    (void)rc_gzclose;
    (void)rc_def_end1;
    (void)rc_def_end2;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}