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
//<ID> 1364
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload demonstrating deflateSetHeader/inflateGetHeader/inflateGetDictionary/adler32_z";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    Bytef *decBuf = new Bytef[(size_t)srcLen];
    memset(decBuf, 0, (size_t)srcLen);

    // step 2: Setup (initialize deflate, set gzip header, compress)
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header head_out;
    memset(&head_out, 0, (size_t)sizeof(gz_header));
    head_out.text = 1;
    head_out.time = (uLong)123456789UL;
    head_out.xflags = 2;
    head_out.os = 255;
    int rc_set_header = deflateSetHeader(&dstrm, &head_out);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;
    int rc_deflate = deflate(&dstrm, 4);
    uLong compProduced = dstrm.total_out;

    // step 3: Operate and validate (initialize inflate, retrieve header and dictionary, inflate, compute adler)
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compProduced;
    istrm.next_out = decBuf;
    istrm.avail_out = (uInt)srcLen;
    gz_header head_in;
    memset(&head_in, 0, (size_t)sizeof(gz_header));
    uInt dictLen = 0;
    Bytef *dictBuf = new Bytef[64];
    memset(dictBuf, 0, 64);
    int rc_get_dict = inflateGetDictionary(&istrm, dictBuf, &dictLen);
    int rc_get_header = inflateGetHeader(&istrm, &head_in);
    int rc_inflate = inflate(&istrm, 0);
    uLong adler_after = adler32_z(0UL, decBuf, (z_size_t)srcLen);

    // step 4: Cleanup and finalization
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] src;
    delete [] compBuf;
    delete [] decBuf;
    delete [] dictBuf;
    (void)version;
    (void)bound;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)rc_deflate;
    (void)compProduced;
    (void)rc_inflate_init;
    (void)rc_get_dict;
    (void)rc_get_header;
    (void)rc_inflate;
    (void)adler_after;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}