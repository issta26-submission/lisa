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
//<ID> 1265
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "Example payload for zlib sequence usage";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    uLong crc_val = crc32(0UL, src, (uInt)srcLen);
    uLong ad1 = adler32(1UL, src, (uInt)srcLen);
    uLong ad2 = adler32(1UL, src, (uInt)srcLen);
    uLong ad_combined = adler32_combine(ad1, ad2, (off_t)srcLen);

    // step 2: Setup (compress source and prepare inflate stream)
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen_f = (uLongf)bound;
    int rc_compress = compress(compBuf, &compLen_f, src, srcLen);
    uLong compLen = (uLong)compLen_f;
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;

    // step 3: Core operations (initialize inflate, get gzip header, write some bytes to gz file)
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    int rc_inflate_get_header = inflateGetHeader(&istrm, &head);
    gzFile gzf = gzopen("zlib_seq_output.gz", "wb");
    int rc_gzput0 = gzputc(gzf, (int)compBuf[0]);
    int rc_gzput1 = gzputc(gzf, (int)compBuf[1]);

    // step 4: Cleanup and finalization
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gzf);
    delete [] src;
    delete [] compBuf;
    (void)version;
    (void)crc_val;
    (void)ad_combined;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate_get_header;
    (void)rc_gzput0;
    (void)rc_gzput1;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}