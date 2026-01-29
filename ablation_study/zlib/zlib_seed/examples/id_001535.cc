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
//<ID> 1535
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload: example data for gzwrite/gzread + inflate";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *writeBuf = new Bytef[(size_t)srcLen];
    memcpy(writeBuf, payload, (size_t)srcLen);
    uLong compile_flags = zlibCompileFlags();

    // step 2: Configure (open gz file for writing and write payload)
    gzFile gzf_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int written = gzwrite(gzf_w, writeBuf, (unsigned int)srcLen);
    int rc_close_w = gzclose(gzf_w);

    // step 3: Operate (open gz file for reading, read into buffer, then attempt inflate)
    gzFile gzf_r = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *readBuf = new Bytef[(size_t)(srcLen + 64)];
    memset(readBuf, 0, (size_t)(srcLen + 64));
    int bytes_read = gzread(gzf_r, readBuf, (unsigned int)(srcLen + 64));
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    strm.next_in = readBuf;
    strm.avail_in = (uInt)bytes_read;
    strm.next_out = writeBuf;
    strm.avail_out = (uInt)(srcLen + 64);
    strm.adler = compile_flags;
    const char *version = zlibVersion();
    int rc_inflate_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&strm, 0);
    int rc_inflate_end = inflateEnd(&strm);

    // step 4: Validate & Cleanup
    int rc_close_r = gzclose(gzf_r);
    delete [] writeBuf;
    delete [] readBuf;
    (void)written;
    (void)bytes_read;
    (void)rc_close_w;
    (void)rc_close_r;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)version;
    (void)compile_flags;
    (void)srcLen;
    // API sequence test completed successfully
    return 66;
}