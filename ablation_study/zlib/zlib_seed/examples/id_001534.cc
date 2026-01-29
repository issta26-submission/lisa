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
//<ID> 1534
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload: example data to compress and read back";
    const uLong payloadLen = (uLong)(sizeof(payload) - 1);
    Bytef *writeBuf = new Bytef[(size_t)payloadLen];
    memset(writeBuf, 0, (size_t)payloadLen);
    memcpy(writeBuf, payload, (size_t)payloadLen);
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags;

    // step 2: Configure (create gz file and write payload)
    const char *fname = "test_zlib_api_sequence_stream.gz";
    gzFile gzf_write = gzopen(fname, "wb");
    int wrote = gzwrite(gzf_write, writeBuf, (unsigned int)payloadLen);
    (void)wrote;
    int rc_close_write = gzclose(gzf_write);
    (void)rc_close_write;

    // step 3: Operate (open gz for read, read into buffer, inflate the read bytes)
    gzFile gzf_read = gzopen(fname, "rb");
    const unsigned int readBufSize = (unsigned int)(payloadLen + 16);
    Bytef *readBuf = new Bytef[(size_t)readBufSize];
    memset(readBuf, 0, (size_t)readBufSize);
    int bytes_read = gzread(gzf_read, readBuf, readBufSize);
    (void)bytes_read;
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    const char *ver = zlibVersion();
    int rc_inflate_init = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    (void)rc_inflate_init;
    strm.next_in = readBuf;
    strm.avail_in = (uInt)((bytes_read > 0) ? (uInt)bytes_read : 0u);
    const unsigned int outBufSize = 4096u;
    Bytef *outBuf = new Bytef[(size_t)outBufSize];
    memset(outBuf, 0, (size_t)outBufSize);
    strm.next_out = outBuf;
    strm.avail_out = outBufSize;
    int rc_inflate = inflate(&strm, 0);
    (void)rc_inflate;

    // step 4: Validate and Cleanup
    int rc_close_read = gzclose(gzf_read);
    (void)rc_close_read;
    int rc_inflate_end = inflateEnd(&strm);
    (void)rc_inflate_end;
    delete [] writeBuf;
    delete [] readBuf;
    delete [] outBuf;
    // API sequence test completed successfully
    return 66;
}