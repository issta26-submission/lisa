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
//<ID> 970
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    Bytef *read_buf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(read_buf, 0, (size_t)ORIG_LEN);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp_buf, 0, (size_t)ORIG_LEN);

    // step 2: Create a gz file and write data, then close writer with gzclose_w
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzclose_w(gw);

    // step 3: Open gz for reading, read data with gzread, initialize inflate and validate
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    int read_ret = gzread(gr, read_buf, (unsigned int)ORIG_LEN);
    gzclose(gr);
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)read_ret;
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflateValidate(&istrm, 1);

    // step 4: Cleanup
    inflateEnd(&istrm);
    free(orig);
    free(read_buf);
    free(decomp_buf);
    (void)ver;
    (void)read_ret;
    // API sequence test completed successfully
    return 66;
}