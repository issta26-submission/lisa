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
//<ID> 1398
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
    Bytef *compBuf = new Bytef[(size_t)compBound1];
    memset(compBuf, 0, (size_t)compBound1);
    uLongf compLen = compBound1;
    int rc_compress = compress(compBuf, &compLen, src, srcLen);

    // step 2: Setup deflate stream to further process compressed data
    uLong defBound = compressBound(compLen);
    Bytef *defBuf = new Bytef[(size_t)defBound];
    memset(defBuf, 0, (size_t)defBound);
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    dstrm.next_in = compBuf;
    dstrm.avail_in = (uInt)compLen;
    dstrm.next_out = defBuf;
    dstrm.avail_out = (uInt)defBound;
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 3: Core operations - deflate then prepare inflate and validate
    int rc_deflate = deflate(&dstrm, 0);
    uLong deflatedSize = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    istrm.next_in = defBuf;
    istrm.avail_in = (uInt)deflatedSize;
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inflate_validate = inflateValidate(&istrm, 15);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Write/inspect via gz interface and cleanup
    gzFile gzf = gzdopen(1, "wb");
    off64_t gz_off = gzoffset64(gzf);
    int rc_gzclose = gzclose(gzf);

    delete [] src;
    delete [] compBuf;
    delete [] defBuf;
    delete [] outBuf;
    (void)version;
    (void)compBound1;
    (void)rc_compress;
    (void)defBound;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)deflatedSize;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_inflate_validate;
    (void)rc_inflate_end;
    (void)gzf;
    (void)gz_off;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}