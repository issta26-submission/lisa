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
//<ID> 1329
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib fuzz payload for api sequence using inflate and crc32_z and gzgetc_";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen_f = (uLongf)bound;
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);

    // step 2: Setup (compress source, initialize inflate stream)
    int rc_compress = compress2(compBuf, &compLen_f, src, srcLen, 6);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen_f;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;

    // step 3: Operate (validate before inflate, inflate, compute crc, write gz file, read first byte)
    int rc_validate_pre = inflateValidate(&istrm, 1);
    int rc_inflate = inflate(&istrm, 0);
    uLong crc_src = crc32_z(0UL, src, (z_size_t)srcLen);
    uLong crc_out = crc32_z(0UL, outBuf, (z_size_t)srcLen);
    gzFile gzf_w = gzopen("zlib_api_seq_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf_w, (voidpc)compBuf, (unsigned int)compLen_f);
    int rc_gzclose_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen("zlib_api_seq_out.gz", "rb");
    int first_byte = gzgetc_(gzf_r);

    // step 4: Validate and Cleanup
    int rc_validate_post = inflateValidate(&istrm, 0);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose_r = gzclose(gzf_r);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)version;
    (void)bound;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_validate_pre;
    (void)rc_inflate;
    (void)crc_src;
    (void)crc_out;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)first_byte;
    (void)rc_validate_post;
    (void)rc_inflate_end;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}