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
//<ID> 1262
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "Zlib API sequence payload";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    int rc_get_header = inflateGetHeader(&istrm, &head);

    // step 2: Setup (open gz file and write bytes)
    gzFile gzf = gzopen("zlib_seq_output.gz", "wb");
    int rc_gzputc1 = gzputc(gzf, 'A');
    int rc_gzputc2 = gzputc(gzf, '\n');

    // step 3: Core operations (compute CRC and combine Adler checksums)
    uLong crc = crc32(0UL, src, (uInt)srcLen);
    uInt half = (uInt)(srcLen / 2U);
    uLong ad1 = adler32(1UL, src, half);
    uLong ad2 = adler32(1UL, src + half, (uInt)(srcLen - half));
    uLong ad_comb = adler32_combine(ad1, ad2, (off_t)(srcLen - half));

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    (void)version;
    (void)rc_inflate_init;
    (void)rc_get_header;
    (void)rc_gzputc1;
    (void)rc_gzputc2;
    (void)crc;
    (void)ad1;
    (void)ad2;
    (void)ad_comb;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}