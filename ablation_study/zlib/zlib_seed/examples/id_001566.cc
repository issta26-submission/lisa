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
//<ID> 1566
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib sequence payload for deflateBound, inflateResetKeep, inflateUndermine and gztell";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    z_stream def_strm;
    memset(&def_strm, 0, (size_t)sizeof(z_stream));
    def_strm.next_in = src;
    def_strm.avail_in = (uInt)srcLen;

    // step 2: Configure (compute bounds and prepare buffers, init inflate)
    uLong compBound = deflateBound(&def_strm, srcLen);
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)compBound;
    Bytef *outBuf = new Bytef[payloadLen];
    memset(outBuf, 0, payloadLen);
    z_stream inf_strm;
    memset(&inf_strm, 0, (size_t)sizeof(z_stream));
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = 0;
    inf_strm.next_out = outBuf;
    inf_strm.avail_out = (uInt)srcLen;
    const char *version = zlibVersion();
    int rc_inflate_init = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 3: Operate (write data to gz file, query position, reset and undermine inflate)
    const char *fname = "test_zlib_api_sequence_out.gz";
    gzFile gzf = gzopen(fname, "wb");
    int written = gzwrite(gzf, src, (unsigned int)srcLen);
    off_t pos_after_write = gztell(gzf);
    int rc_reset_keep = inflateResetKeep(&inf_strm);
    int rc_undermine = inflateUndermine(&inf_strm, 1);

    // step 4: Validate and Cleanup
    int rc_inflate_end = inflateEnd(&inf_strm);
    int rc_close = gzclose(gzf);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)compBound;
    (void)rc_inflate_init;
    (void)written;
    (void)pos_after_write;
    (void)rc_reset_keep;
    (void)rc_undermine;
    (void)rc_inflate_end;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}