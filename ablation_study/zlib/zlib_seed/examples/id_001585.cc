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
//<ID> 1585
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
    const char *version = zlibVersion();
    uLong bound = deflateBound(&def_strm, srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)bound;
    int rc_deflate_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int rc_deflate = deflate(&def_strm, 4);
    uLong compSize = def_strm.total_out;
    int rc_deflate_end = deflateEnd(&def_strm);

    // step 2: Configure (write original payload to a gz file, flush and inspect direct mode)
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int written = gzwrite(gzf, src, (unsigned int)srcLen);
    int rc_gzflush = gzflush(gzf, 2);
    int rc_gzdirect = gzdirect(gzf);
    int rc_gzclose_w = gzclose(gzf);

    // step 3: Operate (reopen for reading, rewind and read back)
    gzFile gzf_read = gzopen(fname, "rb");
    int rc_gzrewind = gzrewind(gzf_read);
    Bytef *readBuf = new Bytef[payloadLen + 16];
    memset(readBuf, 0, payloadLen + 16);
    int read_ret = gzread(gzf_read, readBuf, (unsigned int)(payloadLen + 16));
    off_t pos = gztell(gzf_read);

    // step 4: Validate and Cleanup
    int rc_gzclose_r = gzclose(gzf_read);
    delete [] src;
    delete [] compBuf;
    delete [] readBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)bound;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)compSize;
    (void)rc_deflate_end;
    (void)written;
    (void)rc_gzflush;
    (void)rc_gzdirect;
    (void)rc_gzclose_w;
    (void)rc_gzrewind;
    (void)read_ret;
    (void)pos;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}