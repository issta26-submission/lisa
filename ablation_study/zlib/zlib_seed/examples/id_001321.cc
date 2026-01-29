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
//<ID> 1321
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib fuzz payload for api sequence - validate and crc32_z usage";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen_f = (uLongf)bound;

    // step 2: Setup (compress source and initialize inflate stream)
    int rc_compress = compress2(compBuf, &compLen_f, src, srcLen, 6);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen_f;
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;

    // step 3: Core operations (inflate, validate, compute crc, write gz and read a byte)
    int rc_inflate = inflate(&istrm, 0);
    int rc_validate = inflateValidate(&istrm, 1);
    uLong crc = crc32_z(0UL, outBuf, (z_size_t)srcLen);
    gzFile gzf_write = gzopen("zlib_api_seq_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf_write, (voidpc)compBuf, (unsigned int)compLen_f);
    int rc_gzflush = gzflush(gzf_write, 0);
    int rc_gzclose_write = gzclose(gzf_write);
    gzFile gzf_read = gzopen("zlib_api_seq_out.gz", "rb");
    int ch = gzgetc_(gzf_read);
    int rc_gzclose_read = gzclose(gzf_read);

    // step 4: Cleanup and finalization
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)version;
    (void)bound;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_validate;
    (void)crc;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose_write;
    (void)ch;
    (void)rc_gzclose_read;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}