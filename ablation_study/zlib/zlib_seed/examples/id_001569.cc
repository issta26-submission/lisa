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
//<ID> 1569
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload demonstrating deflateBound, inflateResetKeep, inflateUndermine and gztell";
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
    compress2(compBuf, &compLen, src, srcLen, 6);

    // step 2: Configure (write compressed bytes to gz file and open for read)
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int written = gzwrite(gzf, compBuf, (unsigned int)compLen);
    int rc_close_write = gzclose(gzf);
    gzFile gzf_read = gzopen(fname, "rb");
    off_t pos = gztell(gzf_read);

    // step 3: Operate (read back the compressed-bytes payload and operate on inflate stream)
    Bytef *readBuf = new Bytef[(size_t)compLen];
    memset(readBuf, 0, (size_t)compLen);
    int read_ret = gzread(gzf_read, readBuf, (unsigned int)compLen);
    z_stream inf_strm;
    memset(&inf_strm, 0, (size_t)sizeof(z_stream));
    inf_strm.next_in = readBuf;
    inf_strm.avail_in = (uInt)read_ret;
    Bytef *outBuf = new Bytef[payloadLen + 64];
    memset(outBuf, 0, payloadLen + 64);
    inf_strm.next_out = outBuf;
    inf_strm.avail_out = (uInt)(payloadLen + 64);
    const char *version = zlibVersion();
    int rc_inflate_init = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    int rc_resetkeep = inflateResetKeep(&inf_strm);
    int rc_undermine = inflateUndermine(&inf_strm, 1);
    int rc_inflate_end = inflateEnd(&inf_strm);

    // step 4: Validate and Cleanup
    int rc_close_read = gzclose(gzf_read);
    delete [] src;
    delete [] compBuf;
    delete [] readBuf;
    delete [] outBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)bound;
    (void)compLen;
    (void)written;
    (void)rc_close_write;
    (void)pos;
    (void)read_ret;
    (void)version;
    (void)rc_inflate_init;
    (void)rc_resetkeep;
    (void)rc_undermine;
    (void)rc_inflate_end;
    (void)rc_close_read;
    // API sequence test completed successfully
    return 66;
}