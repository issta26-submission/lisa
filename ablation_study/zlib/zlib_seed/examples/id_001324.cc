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
//<ID> 1324
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for inflate, crc, and gzgetc_";
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

    // step 3: Core operations (inflate, validate, compute CRC, write gz then read one byte with gzgetc_)
    int rc_inflate = inflate(&istrm, 0);
    int rc_validate = inflateValidate(&istrm, 1);
    uLong checksum = crc32_z(0UL, outBuf, (z_size_t)srcLen);
    gzFile gzf_write = gzopen("zlib_api_seq_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gzf_write, (voidpc)compBuf, (unsigned int)compLen_f);
    int rc_gzclose_write = gzclose(gzf_write);
    gzFile gzf_read = gzopen("zlib_api_seq_tmp.gz", "rb");
    int c_read = gzgetc_(gzf_read);

    // step 4: Cleanup and finalization
    int rc_gzclose_read = gzclose(gzf_read);
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
    (void)checksum;
    (void)rc_gzwrite;
    (void)rc_gzclose_write;
    (void)c_read;
    (void)rc_gzclose_read;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}