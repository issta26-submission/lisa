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
//<ID> 1400
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload demonstrating inflateEnd/inflateInit_/deflate/inflateBackEnd/inflate";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLongf comp1Len = (uLongf)compressBound(srcLen);
    Bytef *comp1 = new Bytef[(size_t)comp1Len];
    memset(comp1, 0, (size_t)comp1Len);
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);

    // step 2: Setup - produce a compressed buffer using compress()
    int rc_compress = compress(comp1, &comp1Len, src, srcLen);

    // step 3: Core operations - inflate the compressed buffer, then perform a separate deflate
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    istrm.next_in = comp1;
    istrm.avail_in = (uInt)comp1Len;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&istrm, 0);
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
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 4: Inflate-back initialization and cleanup, then final cleanup of resources
    z_stream bstrm;
    memset(&bstrm, 0, (size_t)sizeof(z_stream));
    const int windowBits = 15;
    unsigned char *window = new unsigned char[(size_t)(1 << 15)];
    memset(window, 0, (size_t)(1 << 15));
    int rc_inflate_back_init = inflateBackInit_(&bstrm, windowBits, window, version, (int)sizeof(z_stream));
    int rc_inflate_back_end = inflateBackEnd(&bstrm);

    delete [] src;
    delete [] comp1;
    delete [] outBuf;
    delete [] defBuf;
    delete [] window;
    (void)version;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_deflate_init;
    (void)defBound;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_inflate_back_init;
    (void)rc_inflate_back_end;
    // API sequence test completed successfully
    return 66;
}