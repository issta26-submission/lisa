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
//<ID> 1584
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload using deflateBound, gzflush, gzdirect, deflateInit_, and gzrewind";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    uLong bound_comp = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound_comp];
    memset(compBuf, 0, (size_t)bound_comp);
    uLongf compLen = bound_comp;
    int rc_compress = compress2(compBuf, &compLen, src, srcLen, 6);

    // step 2: Configure
    z_stream def_strm;
    memset(&def_strm, 0, (size_t)sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_deflate_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound_using_deflate = deflateBound(&def_strm, srcLen);

    // step 3: Operate (write compressed bytes to a gz file, flush, check direct mode, rewind)
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_write = gzwrite(gzf, compBuf, (unsigned int)compLen);
    int rc_flush = gzflush(gzf, 0);
    int is_direct_write = gzdirect(gzf);
    int rc_rewind = gzrewind(gzf);
    int rc_close_write = gzclose(gzf);

    // step 4: Validate and Cleanup
    gzFile gzf_read = gzopen(fname, "rb");
    int is_direct_read = gzdirect(gzf_read);
    int rc_close_read = gzclose(gzf_read);
    int rc_deflate_end = deflateEnd(&def_strm);
    delete [] src;
    delete [] compBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)bound_comp;
    (void)compLen;
    (void)rc_compress;
    (void)rc_deflate_init;
    (void)bound_using_deflate;
    (void)rc_write;
    (void)rc_flush;
    (void)is_direct_write;
    (void)rc_rewind;
    (void)rc_close_write;
    (void)is_direct_read;
    (void)rc_close_read;
    (void)rc_deflate_end;
    (void)version;
    (void)fname;
    // API sequence test completed successfully
    return 66;
}