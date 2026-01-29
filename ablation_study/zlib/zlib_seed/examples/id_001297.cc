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
//<ID> 1297
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for gzprintf and deflateEnd";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen_f = (uLongf)bound;

    // step 2: Setup (produce compressed buffer and prepare a deflate stream to exercise deflateEnd)
    int rc_compress = compress(compBuf, &compLen_f, src, srcLen);
    uLong compLen = (uLong)compLen_f;
    z_stream defstrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));

    // step 3: Core operations (open gzip file, write a textual header with gzprintf and write compressed data)
    gzFile gzf = gzopen("zlib_api_seq_out.gz", "wb");
    int rc_gzprintf = gzprintf(gzf, "Compressed payload length: %lu\n", (unsigned long)compLen);
    int rc_gzwrite = gzwrite(gzf, (voidpc)compBuf, (unsigned int)compLen);

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    int rc_deflate_end = deflateEnd(&defstrm);
    delete [] src;
    delete [] compBuf;
    (void)version;
    (void)bound;
    (void)compLen_f;
    (void)rc_compress;
    (void)rc_deflate_init;
    (void)rc_gzprintf;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}