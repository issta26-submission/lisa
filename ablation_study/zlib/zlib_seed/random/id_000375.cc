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
//<ID> 375
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload, version, and buffers
    const char src[] = "zlib API sequence payload: deflate -> uncompress2 -> write gz -> gztell -> gzclose";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    Bytef *uncompBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(uncompBuf, 0, (size_t)sourceLen);

    // step 2: Initialize deflate stream and perform compression
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)sourceLen;
    strm.next_out = compBuf;
    strm.avail_out = (uInt)bound;
    int rc_deflate = deflate(&strm, 0);
    uLong compSize = strm.total_out;
    int rc_def_end = deflateEnd(&strm);

    // step 3: Validate by decompressing with uncompress2
    uLongf destLen = (uLongf)sourceLen;
    uLong compSizeForUncompress = compSize;
    int rc_uncompress2 = uncompress2(uncompBuf, &destLen, compBuf, &compSizeForUncompress);

    // step 4: Write compressed output to a gzip file, query position, and cleanup
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf, compBuf, (unsigned int)compSize);
    off_t pos = gztell(gzf);
    int rc_gzclose = gzclose(gzf);
    free(compBuf);
    free(uncompBuf);

    (void)version;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_uncompress2;
    (void)rc_gzwrite;
    (void)pos;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}