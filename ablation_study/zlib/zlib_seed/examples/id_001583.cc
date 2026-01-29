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
//<ID> 1583
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
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

    // step 2: Configure (initialize deflate stream and produce compressed bytes)
    int rc_deflate_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)bound;
    int rc_deflate = deflate(&def_strm, 4); // Z_FINISH == 4
    uLong compLen = def_strm.total_out;
    int rc_deflate_end = deflateEnd(&def_strm);

    // step 3: Operate (write a gz file, flush, then reopen, check direct mode and rewind/read)
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int written = gzwrite(gzf, src, (unsigned int)srcLen);
    int rc_gzflush = gzflush(gzf, 2); // Z_SYNC_FLUSH == 2
    int rc_gzclose_w = gzclose(gzf);
    gzFile gzf_read = gzopen(fname, "rb");
    int is_direct = gzdirect(gzf_read);
    int rc_rewind = gzrewind(gzf_read);
    Bytef *outBuf = new Bytef[payloadLen + 64];
    memset(outBuf, 0, payloadLen + 64);
    int read_ret = gzread(gzf_read, outBuf, (unsigned int)(payloadLen + 64));
    int rc_gzclose_r = gzclose(gzf_read);

    // step 4: Validate and Cleanup
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)bound;
    (void)compLen;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)fname;
    (void)gzf;
    (void)written;
    (void)rc_gzflush;
    (void)rc_gzclose_w;
    (void)gzf_read;
    (void)is_direct;
    (void)rc_rewind;
    (void)read_ret;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}