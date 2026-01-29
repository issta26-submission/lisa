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
//<ID> 1416
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Step 1: Declarations
    const char payload[] = "zlib api sequence payload for deflateInit2_/inflateInit2_/inflateGetHeader/gzopen/gzclose_w";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();

    // Step 2: Setup - initialize deflate with gzip wrapper and prepare buffers
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));
    uLong defBound = deflateBound(&dstrm, srcLen);
    Bytef *defBuf = new Bytef[(size_t)defBound];
    memset(defBuf, 0, (size_t)defBound);
    dstrm.next_out = defBuf;
    dstrm.avail_out = (uInt)defBound;

    // Step 3: Core operations - perform deflate to produce gzip data, then inflate it and retrieve gzip header
    int rc_deflate = deflate(&dstrm, 4); // 4 -> Z_FINISH
    uLong compLen = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);
    istrm.next_in = defBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate_init = inflateInit2_(&istrm, 47, version, (int)sizeof(z_stream)); // 47 = 15 + 32 to auto-detect gzip/zlib
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    int rc_inflate_get_header = inflateGetHeader(&istrm, &head);
    int rc_inflate = inflate(&istrm, 0);
    uLong outSize = istrm.total_out;
    int rc_inflate_end = inflateEnd(&istrm);

    // Step 4: Cleanup - write original data to a gz file and close write-only gz handle, then free resources
    gzFile gzf = gzopen("test_output_example.gz", "wb");
    int rc_gzwrite = 0;
    if (gzf) rc_gzwrite = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);
    int rc_gzclose_w = gzclose_w(gzf);

    delete [] src;
    delete [] defBuf;
    delete [] outBuf;
    (void)version;
    (void)rc_deflate_init;
    (void)defBound;
    (void)rc_deflate;
    (void)compLen;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_inflate_get_header;
    (void)rc_inflate;
    (void)outSize;
    (void)rc_inflate_end;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    // API sequence test completed successfully
    return 66;
}