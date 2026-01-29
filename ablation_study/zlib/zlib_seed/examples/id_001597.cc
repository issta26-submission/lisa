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
//<ID> 1597
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib_api_sequence_payload_for_checksums_and_gzputc";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    z_stream def_strm;
    memset(&def_strm, 0, (size_t)sizeof(z_stream));
    def_strm.next_in = src;
    def_strm.avail_in = (uInt)srcLen;
    def_strm.zalloc = NULL;
    def_strm.zfree = NULL;
    def_strm.opaque = NULL;
    const char *version = zlibVersion();
    int rc_deflate_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure (open gz file for writing)
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");

    // step 3: Operate (prime deflate bit buffer, write bytes to gz, compute checksums)
    int rc_deflate_prime = deflatePrime(&def_strm, 3, 5);
    int rc_putc = gzputc(gzf, (int)src[0]);
    int rc_gzwrite = gzwrite(gzf, (voidp)(src + 1), (unsigned int)(srcLen - 1));
    uLong adler = adler32_z(1UL, src, (z_size_t)srcLen);
    uLong crc = crc32_z(0UL, src, (z_size_t)srcLen);

    // step 4: Validate and Cleanup
    int rc_gzclose = gzclose(gzf);
    int rc_deflate_end = deflateEnd(&def_strm);
    delete [] src;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate_prime;
    (void)rc_putc;
    (void)rc_gzwrite;
    (void)adler;
    (void)crc;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}