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
//<ID> 1369
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for deflateSetHeader/inflateGetHeader/inflateGetDictionary/adler32_z";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);

    // prepare gzip header to attach to deflate
    gz_header hdr_out;
    memset(&hdr_out, 0, (size_t)sizeof(gz_header));
    Bytef *name_out = new Bytef[16];
    memset(name_out, 0, 16);
    const char name_str[] = "testname";
    memcpy(name_out, name_str, (size_t)(sizeof(name_str) - 1));
    hdr_out.text = 1;
    hdr_out.name = name_out;
    hdr_out.name_max = 16;
    hdr_out.hcrc = 0;

    // step 2: Setup (initialize deflate with gzip wrapper and set header)
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));
    int rc_set_header = deflateSetHeader(&dstrm, &hdr_out);

    // step 3: Core operations (deflate, then inflate and query headers/dictionary, compute adler)
    int rc_deflate = deflate(&dstrm, 0);
    uLong compSize = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compSize;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate_init = inflateInit2_(&istrm, 31, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&istrm, 0);

    gz_header hdr_in;
    memset(&hdr_in, 0, (size_t)sizeof(gz_header));
    Bytef *name_in = new Bytef[64];
    memset(name_in, 0, 64);
    hdr_in.name = name_in;
    hdr_in.name_max = 64;
    Bytef *comment_in = new Bytef[64];
    memset(comment_in, 0, 64);
    hdr_in.comment = comment_in;
    hdr_in.comm_max = 64;
    int rc_get_header = inflateGetHeader(&istrm, &hdr_in);

    uInt dictBufSize = 32;
    Bytef *dictBuf = new Bytef[(size_t)dictBufSize];
    memset(dictBuf, 0, (size_t)dictBufSize);
    uInt dictLen = dictBufSize;
    int rc_get_dict = inflateGetDictionary(&istrm, dictBuf, &dictLen);

    uLong checksum = adler32_z(1UL, outBuf, (z_size_t)istrm.total_out);

    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Cleanup
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    delete [] name_out;
    delete [] name_in;
    delete [] comment_in;
    delete [] dictBuf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)rc_deflate;
    (void)compSize;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_get_header;
    (void)rc_get_dict;
    (void)checksum;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}