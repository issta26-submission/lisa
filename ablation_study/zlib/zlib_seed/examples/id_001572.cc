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
//<ID> 1572
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload using inflateInit2_, compressBound, inflateSync, gzputs and inflateSyncPoint";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = bound;
    compress2(compBuf, &compLen, src, srcLen, 6);

    // step 2: Configure (write gz file with a metadata line and compressed bytes)
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzputs = gzputs(gzf, "metadata: zlib-test\n");
    int written = gzwrite(gzf, compBuf, (unsigned int)compLen);
    int rc_close_write = gzclose(gzf);

    // step 3: Operate (read back data and initialize inflate stream, then test sync and sync point)
    gzFile gzf_read = gzopen(fname, "rb");
    Bytef *read_buf = new Bytef[(size_t)compLen];
    memset(read_buf, 0, (size_t)compLen);
    int read_ret = gzread(gzf_read, read_buf, (unsigned int)compLen);
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    strm.next_in = read_buf;
    strm.avail_in = (uInt)read_ret;
    Bytef *out_buf = new Bytef[payloadLen + 128];
    memset(out_buf, 0, payloadLen + 128);
    strm.next_out = out_buf;
    strm.avail_out = (uInt)(payloadLen + 128);
    const char *version = zlibVersion();
    int rc_inflate_init = inflateInit2_(&strm, 15 + 32, version, (int)sizeof(z_stream));
    int rc_sync_point = inflateSyncPoint(&strm);
    int rc_sync = inflateSync(&strm);

    // step 4: Validate and Cleanup
    int rc_inflate_end = inflateEnd(&strm);
    int rc_close_read = gzclose(gzf_read);
    delete [] src;
    delete [] compBuf;
    delete [] read_buf;
    delete [] out_buf;
    (void)payloadLen;
    (void)srcLen;
    (void)bound;
    (void)compLen;
    (void)rc_gzputs;
    (void)written;
    (void)rc_close_write;
    (void)read_ret;
    (void)version;
    (void)rc_inflate_init;
    (void)rc_sync_point;
    (void)rc_sync;
    (void)rc_inflate_end;
    (void)rc_close_read;
    // API sequence test completed successfully
    return 66;
}