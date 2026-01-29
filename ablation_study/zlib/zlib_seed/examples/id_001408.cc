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
//<ID> 1408
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload demonstrating inflateInit_/inflate/inflateBackEnd/deflate";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();

    // step 2: Setup - create a valid compressed buffer using compress()
    uLongf compLen = (uLongf)compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)compLen];
    memset(compBuf, 0, (size_t)compLen);
    int rc_compress = compress(compBuf, &compLen, src, srcLen);

    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);

    // step 3: Core operations - inflate the compressed buffer, use inflateBackInit_/inflateBackEnd, and perform a deflate pass
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_end = inflateEnd(&istrm);

    // inflate back init and end on a separate stream (provide a working window buffer)
    z_stream bstrm;
    memset(&bstrm, 0, (size_t)sizeof(z_stream));
    unsigned char *windowBuf = new unsigned char[32768];
    memset(windowBuf, 0, 32768);
    int rc_back_init = inflateBackInit_(&bstrm, 15, windowBuf, version, (int)sizeof(z_stream));
    int rc_back_end = inflateBackEnd(&bstrm);

    // Initialize a deflate stream and perform a single deflate call
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong defBound = deflateBound(&dstrm, srcLen);
    Bytef *defBuf = new Bytef[(size_t)defBound];
    memset(defBuf, 0, (size_t)defBound);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = defBuf;
    dstrm.avail_out = (uInt)defBound;
    int rc_deflate = deflate(&dstrm, 0);
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 4: Cleanup
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    delete [] windowBuf;
    delete [] defBuf;
    (void)version;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_back_init;
    (void)rc_back_end;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}