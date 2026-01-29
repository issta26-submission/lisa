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
//<ID> 850
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers, compress data and write to a gz file
    const char *ver = zlibVersion();
    const uLong SRC_LEN = 64UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'A', (size_t)SRC_LEN);
    uLong adl_init = adler32(1UL, src, (uInt)SRC_LEN);

    uLong compCap = 128UL;
    Bytef *comp = (Bytef *)malloc((size_t)compCap);
    memset(comp, 0, (size_t)compCap);

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)SRC_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)compCap;

    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    deflate(&dstrm, 4); /* Z_FINISH */

    gzFile out_gz = gzopen("tmp_zlib_test.gz", "wb");
    gzwrite(out_gz, comp, (unsigned int)dstrm.total_out);
    gzclose(out_gz);

    // step 2: Setup inflate stream and read compressed data back via gzread
    gzFile in_gz = gzopen("tmp_zlib_test.gz", "rb");
    Bytef *gzbuf = (Bytef *)malloc((size_t)compCap);
    memset(gzbuf, 0, (size_t)compCap);
    int bytesRead = gzread(in_gz, gzbuf, (unsigned int)dstrm.total_out);

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    istrm.next_in = gzbuf;
    istrm.avail_in = (uInt)bytesRead;
    istrm.adler = adl_init;
    inflateInit2_(&istrm, 15, ver, (int)sizeof(z_stream));

    // step 3: Core operations — inflate and validate with adler32
    Bytef *outbuf = (Bytef *)malloc((size_t)SRC_LEN);
    memset(outbuf, 0, (size_t)SRC_LEN);
    istrm.next_out = outbuf;
    istrm.avail_out = (uInt)SRC_LEN;
    inflate(&istrm, 0); /* Z_NO_FLUSH */
    uLong adl_out = adler32(0UL, outbuf, (uInt)istrm.total_out);

    // step 4: Cleanup
    inflateEnd(&istrm);
    deflateEnd(&dstrm);
    gzclose(in_gz);
    free(src);
    free(comp);
    free(gzbuf);
    free(outbuf);
    (void)ver;
    (void)adl_init;
    (void)adl_out;
    (void)bytesRead;
    // API sequence test completed successfully
    return 66;
}