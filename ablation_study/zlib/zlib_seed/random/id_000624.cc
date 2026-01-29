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
//<ID> 624
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare source + buffers
    const char src[] = "zlib API sequence test payload for combined APIs";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLongf compBufCap = (uLongf)(sourceLen + 128);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufCap);
    memset(compBuf, 0, (size_t)compBufCap);

    // step 2: Initialize streams (deflate source, copy target, and inflate) and do one-shot compress
    z_stream dstrm;
    z_stream dstrm_copy;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(z_stream));
    memset(&dstrm_copy, 0, sizeof(z_stream));
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBufCap;
    uLongf compLen = compBufCap;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 3: Operate - write compressed bytes to gzFile, query EOF, perform deflate copy, init inflate
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)compLen);
    int rc_gzeof = gzeof(gf);
    int rc_defcopy = deflateCopy(&dstrm_copy, &dstrm);
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 4: Validate & Cleanup
    int rc_def_end_src = deflateEnd(&dstrm);
    int rc_def_end_copy = deflateEnd(&dstrm_copy);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gf);
    free(compBuf);
    (void)rc_def_init;
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gzeof;
    (void)rc_defcopy;
    (void)rc_inflate_init;
    (void)rc_def_end_src;
    (void)rc_def_end_copy;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}