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
//<ID> 374
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, buffers and compute compressed size
    const char src[] = "zlib API sequence payload: compress2 -> write gz -> tell -> uncompress2";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);

    // step 2: One-shot compression and initialize a deflate stream (for API coverage)
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&strm, 6, zlibVersion(), (int)sizeof(z_stream));
    int rc_def_end = deflateEnd(&strm);

    // step 3: Write compressed blob to a gzip file, query file offset, then close
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)compBuf, (unsigned int)compLen);
    off_t pos = gztell(gzf);
    int rc_gzclose = gzclose(gzf);

    // step 4: Decompress the compressed buffer with uncompress2 and cleanup
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    uLongf outLen = (uLongf)sourceLen;
    uLong compLen_for_uncompress = (uLong)compLen;
    int rc_uncomp = uncompress2(outBuf, &outLen, compBuf, &compLen_for_uncompress);
    free(compBuf);
    free(outBuf);

    (void)rc_comp2;
    (void)rc_def_init;
    (void)rc_def_end;
    (void)rc_gzwrite;
    (void)pos;
    (void)rc_gzclose;
    (void)rc_uncomp;
    // API sequence test completed successfully
    return 66;
}