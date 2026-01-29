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
//<ID> 1322
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for inflate, crc and gzgetc_";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen_f = (uLongf)bound;

    // step 2: Setup
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
    gzFile gzf_w = gzopen("zlib_api_seq_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf_w, (voidpc)compBuf, (unsigned int)compLen_f);
    int rc_gzflush = gzflush(gzf_w, 0);
    int rc_gzclose_w = gzclose(gzf_w);

    // step 3: Core operations and validation
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_validate = inflateValidate(&istrm, 1);
    uLong checksum_full = crc32_z(0UL, outBuf, (z_size_t)srcLen);
    gzFile gzf_r = gzopen("zlib_api_seq_out.gz", "rb");
    int first_char = gzgetc_(gzf_r);
    int second_char = gzgetc_(gzf_r);
    uLong checksum_first_two = crc32_z(0UL, (const Bytef *)(unsigned char[]){(Byte)first_char, (Byte)second_char}, (z_size_t)2);

    // step 4: Cleanup and finalization
    int rc_gzclose_r = gzclose(gzf_r);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)version;
    (void)bound;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose_w;
    (void)rc_inflate;
    (void)rc_inflate_validate;
    (void)checksum_full;
    (void)first_char;
    (void)second_char;
    (void)checksum_first_two;
    (void)rc_gzclose_r;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}