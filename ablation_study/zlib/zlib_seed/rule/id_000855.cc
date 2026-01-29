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
//<ID> 855
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong IN_LEN = 512UL;
    Bytef *in_buf = (Bytef *)malloc((size_t)IN_LEN);
    memset(in_buf, 'B', (size_t)IN_LEN);
    uLong checksum = adler32(0UL, in_buf, (uInt)IN_LEN);

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;

    // step 2: Initialize inflate and deflate streams
    int windowBits = 15;
    inflateInit2_(&istrm, windowBits, ver, (int)sizeof(z_stream));
    istrm.next_in = in_buf;
    istrm.avail_in = (uInt)IN_LEN;
    istrm.adler = checksum;
    Bytef *inflate_out = (Bytef *)malloc((size_t)(IN_LEN * 2));
    memset(inflate_out, 0, (size_t)(IN_LEN * 2));
    istrm.next_out = inflate_out;
    istrm.avail_out = (uInt)(IN_LEN * 2);

    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));

    // step 3: Core operations - inflate, then deflate, and perform a gz read
    inflate(&istrm, 0); /* Z_NO_FLUSH == 0 */
    uLong bound = deflateBound(&dstrm, IN_LEN);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    dstrm.next_in = in_buf;
    dstrm.avail_in = (uInt)IN_LEN;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4); /* Z_FINISH == 4 */

    gzFile gf = gzopen("test_zlib_api_seq.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc(128U);
    memset(read_buf, 0, 128U);
    gzread(gf, read_buf, 128U);

    // step 4: Cleanup and finalize
    gzclose(gf);
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(in_buf);
    free(inflate_out);
    free(comp_buf);
    free(read_buf);
    (void)ver;
    (void)checksum;
    (void)bound;
    // API sequence test completed successfully
    return 66;
}