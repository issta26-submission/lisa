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
//<ID> 851
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong PLAIN_LEN = 256UL;
    Bytef *plain = (Bytef *)malloc((size_t)PLAIN_LEN);
    memset(plain, 'A', (size_t)PLAIN_LEN);
    uLong plain_adler = adler32(0UL, plain, (uInt)PLAIN_LEN);

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    dstrm.next_in = plain;
    dstrm.avail_in = (uInt)PLAIN_LEN;

    uLong compCap = PLAIN_LEN + 128UL;
    Bytef *comp = (Bytef *)malloc((size_t)compCap);
    memset(comp, 0, (size_t)compCap);
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)compCap;

    // step 2: Initialize deflate and inflate streams
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit2_(&istrm, 15, ver, (int)sizeof(z_stream));

    // step 3: Operate: compress with deflate, then inflate, write/read via gz
    deflate(&dstrm, 4); /* Z_FINISH */
    uLong compUsed = dstrm.total_out;
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compUsed;

    Bytef *out = (Bytef *)malloc((size_t)PLAIN_LEN);
    memset(out, 0, (size_t)PLAIN_LEN);
    istrm.next_out = out;
    istrm.avail_out = (uInt)PLAIN_LEN;
    inflate(&istrm, 4); /* Z_FINISH */

    gzFile gf = gzopen("test_zlib_api_seq.gz", "wb");
    gzwrite(gf, out, (unsigned int)PLAIN_LEN);
    gzclose(gf);
    gzFile gf_r = gzopen("test_zlib_api_seq.gz", "rb");
    Bytef *readbuf = (Bytef *)malloc((size_t)PLAIN_LEN);
    memset(readbuf, 0, (size_t)PLAIN_LEN);
    gzread(gf_r, readbuf, (unsigned int)PLAIN_LEN);
    gzclose(gf_r);

    // step 4: Cleanup
    inflateEnd(&istrm);
    deflateEnd(&dstrm);
    free(plain);
    free(comp);
    free(out);
    free(readbuf);
    (void)ver;
    (void)plain_adler;
    (void)compUsed;
    // API sequence test completed successfully
    return 66;
}