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
//<ID> 1317
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib_api_sequence_payload_for_inflate_and_gzopen64";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    uLong checksum_src = adler32(0UL, src, (uInt)srcLen);
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Setup (compress data and initialize inflate stream)
    int rc_compress = compress2(compBuf, &compLen, src, srcLen, 6);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Configure and Operate (configure stream, reset window size, inflate, write gz file and seek)
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate_reset2 = inflateReset2(&istrm, 15);
    int rc_inflate = inflate(&istrm, 0);
    gzFile gzf = gzopen64("zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)compBuf, (unsigned int)compLen);
    off_t pos_after_write = gzseek(gzf, 0, 1);

    // step 4: Validate and Cleanup
    uLong checksum_out = adler32(0UL, outBuf, (uInt)srcLen);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gzf);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)checksum_src;
    (void)checksum_out;
    (void)bound;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate_reset2;
    (void)rc_inflate;
    (void)rc_gzwrite;
    (void)pos_after_write;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    (void)version;
    // API sequence test completed successfully
    return 66;
}