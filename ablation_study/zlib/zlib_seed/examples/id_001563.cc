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
//<ID> 1563
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib sequence payload for deflateBound, inflateUndermine, inflateResetKeep and gztell";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    strm.next_in = src;
    strm.avail_in = (uInt)srcLen;
    uLong compBound = deflateBound(&strm, srcLen);
    Bytef *comp = new Bytef[(size_t)compBound];
    memset(comp, 0, (size_t)compBound);

    // step 2: Configure (create gz file, write payload, reopen for read and query position)
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf_write = gzopen(fname, "wb");
    int written = gzwrite(gzf_write, src, (unsigned int)srcLen);
    int rc_close_write = gzclose(gzf_write);
    gzFile gzf_read = gzopen(fname, "rb");
    off_t position_before = gztell(gzf_read);
    Bytef *read_buf = new Bytef[payloadLen];
    memset(read_buf, 0, payloadLen);
    int read_ret = gzread(gzf_read, read_buf, (unsigned int)srcLen);

    // step 3: Operate (initialize inflate, apply undermine and reset-keep, then inflate)
    memset(&strm, 0, (size_t)sizeof(z_stream));
    strm.next_in = read_buf;
    strm.avail_in = (uInt)srcLen;
    Bytef *out_buf = new Bytef[payloadLen];
    memset(out_buf, 0, payloadLen);
    strm.next_out = out_buf;
    strm.avail_out = (uInt)srcLen;
    const char *version = zlibVersion();
    int rc_inflate_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    int rc_undermine = inflateUndermine(&strm, 1);
    int rc_resetkeep = inflateResetKeep(&strm);
    int rc_inflate = inflate(&strm, 0);
    int rc_inflate_end = inflateEnd(&strm);

    // step 4: Validate and Cleanup
    int rc_close_read = gzclose(gzf_read);
    delete [] src;
    delete [] comp;
    delete [] read_buf;
    delete [] out_buf;
    (void)payloadLen;
    (void)srcLen;
    (void)compBound;
    (void)written;
    (void)rc_close_write;
    (void)position_before;
    (void)read_ret;
    (void)version;
    (void)rc_inflate_init;
    (void)rc_undermine;
    (void)rc_resetkeep;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_close_read;
    // API sequence test completed successfully
    return 66;
}