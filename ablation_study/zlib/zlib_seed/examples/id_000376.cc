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
//<ID> 376
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload, version, and buffers
    const char src[] = "zlib API sequence payload: compress2 -> uncompress2 -> deflateInit_ -> gzwrite -> gztell -> gzclose";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);

    // step 2: One-shot compression and decompression using uncompress2
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);
    uLongf decompLen = (uLongf)sourceLen;
    Bytef *decompBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(decompBuf, 0, (size_t)sourceLen);
    uLong compLen_u = (uLong)compLen;
    int rc_uncomp2 = uncompress2(decompBuf, &decompLen, compBuf, &compLen_u);

    // step 3: Initialize deflate stream, query deflate bound, and cleanup deflate state
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    uLong bound2 = deflateBound(&strm, sourceLen);
    int rc_def_end = deflateEnd(&strm);

    // step 4: Write compressed data to a gzip file, get current offset with gztell, close and free resources
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf, compBuf, (unsigned int)compLen_u);
    off_t pos = gztell(gzf);
    int rc_gzclose = gzclose(gzf);
    free(compBuf);
    free(decompBuf);

    (void)version;
    (void)rc_comp2;
    (void)rc_uncomp2;
    (void)decompLen;
    (void)bound2;
    (void)rc_def_init;
    (void)rc_def_end;
    (void)rc_gzwrite;
    (void)pos;
    (void)rc_gzclose;

    // API sequence test completed successfully
    return 66;
}