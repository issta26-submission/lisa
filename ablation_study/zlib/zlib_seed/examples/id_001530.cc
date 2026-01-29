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
//<ID> 1530
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload for inflate + gzread";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong compBound = compressBound(srcLen);
    uLongf compLen = (uLongf)compBound;
    Bytef *comp = new Bytef[(size_t)compBound];
    memset(comp, 0, (size_t)compBound);
    int rc_comp = compress2(comp, &compLen, src, srcLen, 6);

    // step 2: Setup (initialize inflate stream and output buffer)
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    Bytef *out = new Bytef[(size_t)srcLen];
    memset(out, 0, (size_t)srcLen);
    strm.next_out = out;
    strm.avail_out = (uInt)srcLen;

    // step 3: Operate (write and read a gz file, query compile flags, inflate compressed buffer)
    gzFile gzw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int written = gzwrite(gzw, src, (unsigned int)srcLen);
    int rc_close_w = gzclose(gzw);
    gzFile gzr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *readbuf = new Bytef[(size_t)srcLen];
    memset(readbuf, 0, (size_t)srcLen);
    int rc_gzread = gzread(gzr, readbuf, (unsigned int)srcLen);
    uLong flags = zlibCompileFlags();
    int rc_inflate = inflate(&strm, Z_FINISH);

    // step 4: Validate and Cleanup
    int rc_inflate_end = inflateEnd(&strm);
    int rc_close_r = gzclose(gzr);
    delete [] src;
    delete [] comp;
    delete [] out;
    delete [] readbuf;
    (void)version;
    (void)rc_comp;
    (void)rc_inflate_init;
    (void)written;
    (void)rc_close_w;
    (void)rc_gzread;
    (void)flags;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_close_r;
    // API sequence test completed successfully
    return 66;
}