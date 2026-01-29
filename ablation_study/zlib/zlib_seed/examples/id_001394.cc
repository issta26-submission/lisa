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
//<ID> 1394
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for compress/deflate/inflateValidate/gzdopen/gzoffset64";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong compBound1 = compressBound(srcLen);
    uLongf compLen1 = (uLongf)compBound1;
    Bytef *compBuf1 = new Bytef[(size_t)compBound1];
    memset(compBuf1, 0, (size_t)compBound1);
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);

    // step 2: Setup - compress source and prepare inflate stream
    int rc_compress = compress(compBuf1, &compLen1, src, srcLen);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    istrm.next_in = compBuf1;
    istrm.avail_in = (uInt)compLen1;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Core operations - inflate, validate, then deflate, and query gz offset
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_validate = inflateValidate(&istrm, 1);
    int rc_inflate_end = inflateEnd(&istrm);

    uLong compBound2 = compressBound(srcLen);
    Bytef *compBuf2 = new Bytef[(size_t)compBound2];
    memset(compBuf2, 0, (size_t)compBound2);
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf2;
    dstrm.avail_out = (uInt)compBound2;
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_deflate = deflate(&dstrm, 0);
    int rc_deflate_end = deflateEnd(&dstrm);
    uLong deflated_size = dstrm.total_out;

    gzFile gzf = gzdopen(1, "wb");
    off64_t gz_off = gzoffset64(gzf);
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup
    delete [] src;
    delete [] compBuf1;
    delete [] compBuf2;
    delete [] outBuf;
    (void)version;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_validate;
    (void)rc_inflate_end;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)deflated_size;
    (void)gzf;
    (void)gz_off;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}