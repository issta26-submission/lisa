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
//<ID> 1396
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
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);
    Bytef *defBuf = new Bytef[(size_t)compBound];
    memset(defBuf, 0, (size_t)compBound);
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);

    // step 2: Compress source and perform a separate deflate operation
    uLongf compLen = (uLongf)compBound;
    int rc_compress = compress(compBuf, &compLen, src, srcLen);

    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = defBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_deflate = deflate(&dstrm, 0);
    uLong deflatedSize = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Initialize inflate stream, validate it, and inflate compressed data
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inflate_validate = inflateValidate(&istrm, 1);
    int rc_inflate = inflate(&istrm, 0);
    uLong inflatedSize = istrm.total_out;
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Open a gzFile on stdout, write original data, query offset, and cleanup
    gzFile gzf = gzdopen(1, "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);
    off64_t offset64 = gzoffset64(gzf);
    int rc_gzclose = gzclose(gzf);

    // cleanup
    delete [] src;
    delete [] compBuf;
    delete [] defBuf;
    delete [] outBuf;
    (void)version;
    (void)compBound;
    (void)rc_compress;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)deflatedSize;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_inflate_validate;
    (void)rc_inflate;
    (void)inflatedSize;
    (void)rc_inflate_end;
    (void)gzf;
    (void)rc_gzwrite;
    (void)offset64;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}