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
//<ID> 1314
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for inflate, adler32, gzopen64 and gzseek";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen_f = (uLongf)bound;
    const char *version = zlibVersion();

    // step 2: Initialize / Configure
    int rc_compress = compress2(compBuf, &compLen_f, src, srcLen, 6);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inflate_reset2 = inflateReset2(&istrm, 15);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen_f;
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;

    // step 3: Operate / Validate
    int rc_inflate = inflate(&istrm, 0);
    uLong decompressed_bytes = (uLong)istrm.total_out;
    uLong checksum = adler32(0UL, outBuf, (uInt)decompressed_bytes);
    gzFile gzf = gzopen64("zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)outBuf, (unsigned int)decompressed_bytes);
    off_t rc_gzseek = gzseek(gzf, (off_t)0, 0);

    // step 4: Cleanup and finalization
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gzf);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)version;
    (void)bound;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate_reset2;
    (void)rc_inflate;
    (void)checksum;
    (void)rc_gzwrite;
    (void)rc_gzseek;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}