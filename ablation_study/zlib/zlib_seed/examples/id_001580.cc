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
//<ID> 1580
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload demonstrating deflateBound, deflateInit_, gzflush, gzdirect and gzrewind";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    z_stream def_strm;
    memset(&def_strm, 0, (size_t)sizeof(z_stream));
    def_strm.next_in = src;
    def_strm.avail_in = (uInt)srcLen;
    uLong bound = deflateBound(&def_strm, srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = bound;
    int rc_compress = compress2(compBuf, &compLen, src, srcLen, 6);
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure (write compressed bytes to a gz file)
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int written = gzwrite(gzf, compBuf, (unsigned int)compLen);
    int rc_gzflush = gzflush(gzf, 0);
    int rc_gzdirect = gzdirect(gzf);
    int rc_close_write = gzclose(gzf);

    // step 3: Operate (open for read, rewind, and read back compressed bytes)
    gzFile gzr = gzopen(fname, "rb");
    int rc_gzrewind = gzrewind(gzr);
    Bytef *readBuf = new Bytef[(size_t)compLen];
    memset(readBuf, 0, (size_t)compLen);
    int read_ret = gzread(gzr, readBuf, (unsigned int)compLen);
    int rc_close_read = gzclose(gzr);

    // step 4: Validate and Cleanup
    int rc_def_end = deflateEnd(&def_strm);
    delete [] src;
    delete [] compBuf;
    delete [] readBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)bound;
    (void)compLen;
    (void)rc_compress;
    (void)version;
    (void)rc_def_init;
    (void)written;
    (void)rc_gzflush;
    (void)rc_gzdirect;
    (void)rc_close_write;
    (void)rc_gzrewind;
    (void)read_ret;
    (void)rc_close_read;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}