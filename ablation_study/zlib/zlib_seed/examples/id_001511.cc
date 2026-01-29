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
//<ID> 1511
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for gzputs + gzseek64 + gzclearerr + deflateEnd";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    const char *version = zlibVersion();

    // step 2: Setup (initialize deflate stream and prepare output buffer)
    int rc_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    uLong outBound = deflateBound(&strm, srcLen);
    Bytef *out = new Bytef[(size_t)outBound];
    memset(out, 0, (size_t)outBound);
    strm.next_in = src;
    strm.avail_in = (uInt)srcLen;
    strm.next_out = out;
    strm.avail_out = (uInt)outBound;

    // step 3: Core operations (write payload to gz file, seek and clear errors)
    gzFile gzf = gzopen64("test_zlib_api_sequence_stream.gz", "wb");
    int rc_puts = gzputs(gzf, payload);
    off64_t pos = gzseek64(gzf, 0, 0);
    gzclearerr(gzf);

    // step 4: Validate and Cleanup (finalize deflate, close file, free resources)
    int rc_defend = deflateEnd(&strm);
    int rc_close = gzclose(gzf);
    delete [] src;
    delete [] out;
    (void)rc_init;
    (void)rc_puts;
    (void)pos;
    (void)rc_defend;
    (void)rc_close;
    (void)version;
    // API sequence test completed successfully
    return 66;
}