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
//<ID> 1516
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload: deflate + gzputs + gzseek64 + gzclearerr + deflateEnd";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_definit = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Setup (prepare output buffer and stream pointers)
    uLong bound = deflateBound(&strm, srcLen);
    Bytef *compr = new Bytef[(size_t)bound];
    memset(compr, 0, (size_t)bound);
    strm.next_in = src;
    strm.avail_in = (uInt)srcLen;
    strm.next_out = compr;
    strm.avail_out = (uInt)bound;
    int rc_def = deflate(&strm, 4);

    // step 3: Core operations (write a small message to a gzip file and manipulate it)
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_puts = gzputs(gzf, "embedded gzputs message");
    off64_t pos = gzseek64(gzf, 0, 0);
    gzclearerr(gzf);
    int rc_close = gzclose(gzf);

    // step 4: Cleanup and finalize
    int rc_end = deflateEnd(&strm);
    delete [] src;
    delete [] compr;
    (void)version;
    (void)rc_definit;
    (void)bound;
    (void)rc_def;
    (void)rc_puts;
    (void)pos;
    (void)rc_close;
    (void)rc_end;
    // API sequence test completed successfully
    return 66;
}