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
//<ID> 1512
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
    const char *version = zlibVersion();

    // step 2: Setup (open gz file and write)
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_puts = gzputs(gzf, (const char *)src);
    off64_t rc_seek = gzseek64(gzf, (off64_t)0, 0);
    gzclearerr(gzf);

    // step 3: Core operations (initialize deflate, perform a single deflate call, then end)
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int rc_deflate_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&strm, srcLen);
    Bytef *out = new Bytef[(size_t)bound];
    strm.next_in = src;
    strm.avail_in = (uInt)srcLen;
    strm.next_out = out;
    strm.avail_out = (uInt)bound;
    int rc_deflate = deflate(&strm, 0);
    uLong compressedLen = strm.total_out;
    int rc_deflate_end = deflateEnd(&strm);

    // step 4: Validate and Cleanup
    int rc_close = gzclose(gzf);
    delete [] src;
    delete [] out;
    (void)version;
    (void)rc_puts;
    (void)rc_seek;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)compressedLen;
    (void)rc_deflate_end;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}