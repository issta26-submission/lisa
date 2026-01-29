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
//<ID> 1591
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib_api_sequence_demo_payload";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    z_stream def_strm;
    memset(&def_strm, 0, (size_t)sizeof(z_stream));
    def_strm.next_in = src;
    def_strm.avail_in = (uInt)srcLen;
    const char *version = zlibVersion();
    int rc_deflate_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure (compute checksums and prime deflate bit buffer)
    uLong adler = adler32_z(0UL, src, (z_size_t)srcLen);
    uLong crc = crc32_z(0UL, src, (z_size_t)srcLen);
    int prime_bits = 3;
    int prime_value = (int)(crc & 0x7);
    int rc_deflate_prime = deflatePrime(&def_strm, prime_bits, prime_value);

    // step 3: Operate (write a representative byte derived from checksums to a gz file)
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int write_byte = (int)(adler & 0xFF);
    int rc_gzputc = gzputc(gzf, write_byte);
    int rc_gzclose = gzclose(gzf);

    // step 4: Validate and Cleanup
    int rc_deflate_end = deflateEnd(&def_strm);
    delete [] src;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)rc_deflate_init;
    (void)adler;
    (void)crc;
    (void)prime_bits;
    (void)prime_value;
    (void)rc_deflate_prime;
    (void)fname;
    (void)write_byte;
    (void)rc_gzputc;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}