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
//<ID> 1538
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload: test gzopen + gzread + compress2 + inflate";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    uLong compile_flags = zlibCompileFlags();

    // step 2: Configure (create gz file and write payload)
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gw = gzopen(fname, "wb");
    int wrote = gzwrite(gw, src, (unsigned int)srcLen);
    int rc_close_write = gzclose(gw);

    // step 3: Operate (read back with gzread, compress the read data, then inflate it)
    gzFile gr = gzopen(fname, "rb");
    Bytef *readBuf = new Bytef[(size_t)srcLen];
    memset(readBuf, 0, (size_t)srcLen);
    int read_bytes = gzread(gr, readBuf, (unsigned int)srcLen);
    int rc_close_read = gzclose(gr);
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);
    uLong compLen = compBound;
    int rc_comp = compress2(compBuf, &compLen, readBuf, srcLen, 6);

    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    const char *ver = zlibVersion();
    int rc_inflate_init = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    Bytef *outBuf = new Bytef[(size_t)(srcLen + 16)];
    memset(outBuf, 0, (size_t)(srcLen + 16));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compLen;
    strm.next_out = outBuf;
    strm.avail_out = (uInt)(srcLen + 16);
    int rc_inflate = inflate(&strm, 0);
    int rc_inflate_end = inflateEnd(&strm);

    // step 4: Validate and Cleanup
    delete [] src;
    delete [] readBuf;
    delete [] compBuf;
    delete [] outBuf;
    (void)compile_flags;
    (void)wrote;
    (void)rc_close_write;
    (void)read_bytes;
    (void)rc_close_read;
    (void)compBound;
    (void)rc_comp;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}