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
//<ID> 1393
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for compress/inflateValidate/deflate/gzdopen/gzoffset64";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong compBound = compressBound(srcLen);
    uLongf compLen = (uLongf)compBound;
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);

    // step 2: Setup (compress source and prepare inflate stream)
    int rc_compress = compress(compBuf, &compLen, src, srcLen);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inflate_validate = inflateValidate(&istrm, 1);

    // step 3: Core operations (inflate, deflate, gz operations)
    int rc_inflate = inflate(&istrm, 0);
    uLong inflated_size = istrm.total_out;
    int rc_inflate_end = inflateEnd(&istrm);

    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    uLong defCompBound = deflateBound(&dstrm, srcLen);
    Bytef *defCompBuf = new Bytef[(size_t)defCompBound];
    memset(defCompBuf, 0, (size_t)defCompBound);
    dstrm.next_out = defCompBuf;
    dstrm.avail_out = (uInt)defCompBound;
    int rc_deflate = deflate(&dstrm, 0);
    uLong deflated_size = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    gzFile gzf = gzdopen(1, "wb");
    off64_t gzf_off = gzoffset64(gzf);
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    delete [] defCompBuf;
    (void)version;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate_validate;
    (void)rc_inflate;
    (void)inflated_size;
    (void)rc_inflate_end;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)deflated_size;
    (void)rc_deflate_end;
    (void)gzf_off;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}