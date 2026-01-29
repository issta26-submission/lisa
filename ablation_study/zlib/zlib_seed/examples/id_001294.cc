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
//<ID> 1294
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
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen_f = (uLongf)bound;
    int rc_compress = compress(compBuf, &compLen_f, src, srcLen);
    uLong compLen = (uLong)compLen_f;

    // step 2: Setup (initialize a deflate stream to demonstrate init/configure -> end)
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = compBuf;
    dstrm.avail_in = (uInt)compLen;
    Bytef *outBuf = new Bytef[(size_t)(compLen + 32UL)];
    memset(outBuf, 0, (size_t)(compLen + 32UL));
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)(compLen + 32UL);

    // step 3: Core operations (write metadata with gzprintf, write compressed payload, and use stream state)
    gzFile gzf = gzopen("zlib_seq_gzprintf_out.gz", "wb");
    char header[128];
    int header_len = snprintf(header, sizeof(header), "CompressedLen:%lu\n", (unsigned long)compLen);
    (void)header_len;
    gzprintf(gzf, header);
    int rc_gzwrite = gzwrite(gzf, (voidpc)compBuf, (unsigned int)compLen);
    dstrm.total_in = compLen;
    dstrm.total_out = (uLong)(compLen + 1UL);

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)version;
    (void)bound;
    (void)rc_compress;
    (void)rc_deflate_init;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}