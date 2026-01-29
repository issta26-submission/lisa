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
//<ID> 1582
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence using deflateBound, gzflush, gzdirect, deflateInit_, and gzrewind";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    z_stream def_strm;
    memset(&def_strm, 0, (size_t)sizeof(z_stream));
    def_strm.next_in = src;
    def_strm.avail_in = (uInt)srcLen;
    const char *version = zlibVersion();
    int rc_deflate_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = bound;
    int rc_compress = compress2(compBuf, &compLen, src, srcLen, 6);

    // step 2: Configure (write compressed bytes to gz file)
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int written = gzwrite(gzf, compBuf, (unsigned int)compLen);
    int rc_gzflush = gzflush(gzf, 1);
    int rc_gzdirect = gzdirect(gzf);
    int rc_close_write = gzclose(gzf);

    // step 3: Operate (reopen, rewind and read back)
    gzFile gzf_read = gzopen(fname, "rb");
    int rc_gzrewind = gzrewind(gzf_read);
    Bytef *readBuf = new Bytef[(size_t)compLen];
    memset(readBuf, 0, (size_t)compLen);
    int read_ret = gzread(gzf_read, readBuf, (unsigned int)compLen);
    int rc_gzdirect_read = gzdirect(gzf_read);

    // step 4: Validate and Cleanup
    int rc_deflate_end = deflateEnd(&def_strm);
    int rc_close_read = gzclose(gzf_read);
    delete [] src;
    delete [] compBuf;
    delete [] readBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)rc_deflate_init;
    (void)bound;
    (void)compLen;
    (void)rc_compress;
    (void)written;
    (void)rc_gzflush;
    (void)rc_gzdirect;
    (void)rc_close_write;
    (void)rc_gzrewind;
    (void)read_ret;
    (void)rc_gzdirect_read;
    (void)rc_deflate_end;
    (void)rc_close_read;
    // API sequence test completed successfully
    return 66;
}