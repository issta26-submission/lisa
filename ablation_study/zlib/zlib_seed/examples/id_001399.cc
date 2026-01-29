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
//<ID> 1399
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

    // step 2: Setup - compress source with compress()
    uLongf comp1Len = (uLongf)compressBound(srcLen);
    Bytef *comp1 = new Bytef[(size_t)comp1Len];
    memset(comp1, 0, (size_t)comp1Len);
    int rc_compress = compress(comp1, &comp1Len, src, srcLen);

    // step 3: Core operations - inflate the compressed buffer and validate, then deflate the original buffer
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    istrm.next_in = comp1;
    istrm.avail_in = (uInt)comp1Len;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_validate = inflateValidate(&istrm, 1);
    int rc_inflate_end = inflateEnd(&istrm);

    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong defBound = deflateBound(&dstrm, srcLen);
    Bytef *defBuf = new Bytef[(size_t)defBound];
    memset(defBuf, 0, (size_t)defBound);
    dstrm.next_out = defBuf;
    dstrm.avail_out = (uInt)defBound;
    int rc_deflate = deflate(&dstrm, 0);
    uLong defSize = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 4: Finalize and cleanup - write original data to a gz stream opened with gzdopen and query offset
    gzFile gzf = gzdopen(1, "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);
    off64_t gzf_off = gzoffset64(gzf);
    int rc_gzclose = gzclose(gzf);

    // cleanup allocations and silence unused-variable warnings
    delete [] src;
    delete [] comp1;
    delete [] outBuf;
    delete [] defBuf;
    (void)version;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_validate;
    (void)rc_inflate_end;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)defSize;
    (void)rc_deflate_end;
    (void)gzf_off;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}