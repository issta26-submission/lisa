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
//<ID> 377
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload, version and structures
    const char src[] = "zlib API sequence payload: deflate -> write gz -> gztell -> uncompress2";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));

    // step 2: Initialize deflate, compress the payload, finalize deflate
    int rc_def_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&strm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)sourceLen;
    strm.next_out = compBuf;
    strm.avail_out = (uInt)bound;
    int rc_deflate = deflate(&strm, 4); /* Z_FINISH */
    uLong compLen = strm.total_out;
    int rc_def_end = deflateEnd(&strm);

    // step 3: Write compressed data to a gzip file, get file position, and close
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf, compBuf, (unsigned int)compLen);
    off_t pos = gztell(gzf);
    int rc_gzclose = gzclose(gzf);

    // step 4: Uncompress the data using uncompress2 and cleanup
    uLongf destLen = (uLongf)sourceLen;
    Bytef *destBuf = (Bytef *)malloc((size_t)destLen);
    memset(destBuf, 0, (size_t)destLen);
    uLong compLen_in = compLen;
    int rc_uncomp2 = uncompress2(destBuf, &destLen, compBuf, &compLen_in);
    free(compBuf);
    free(destBuf);

    (void)version;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_gzwrite;
    (void)pos;
    (void)rc_gzclose;
    (void)rc_uncomp2;
    (void)compLen;
    (void)bound;

    // API sequence test completed successfully
    return 66;
}