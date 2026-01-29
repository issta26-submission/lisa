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
//<ID> 1517
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload: gzputs + gzseek64 + gzclearerr + deflateEnd";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    const char *version = zlibVersion();
    int rc_deflate_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = src;
    strm.avail_in = (uInt)((srcLen > 0u) ? srcLen : 0u);

    // step 2: Configure
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    (void)rc_deflate_init;

    // step 3: Operate
    int rc_puts = gzputs(gzf, payload);
    off64_t pos_after_write = gzseek64(gzf, (off64_t)0, 0);
    gzclearerr(gzf);

    // step 4: Cleanup and finalize
    int rc_close = gzclose(gzf);
    int rc_deflate_end = deflateEnd(&strm);
    delete [] src;
    (void)rc_puts;
    (void)pos_after_write;
    (void)rc_close;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}