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
//<ID> 973
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare original buffer and write gz file
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    gzFile gf_w = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gf_w, orig, (unsigned int)ORIG_LEN);
    gzclose_w(gf_w);

    // step 2: Read back from the gz file using gzread
    gzFile gf_r = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(read_buf, 0, (size_t)ORIG_LEN);
    int read_bytes = gzread(gf_r, read_buf, (unsigned int)ORIG_LEN);

    // step 3: Initialize an inflate stream and validate it
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    int validate_ret = inflateValidate(&istrm, 0);
    inflateEnd(&istrm);

    // step 4: Compute checksums, cleanup resources
    uLong adler_orig = adler32(0L, orig, (uInt)ORIG_LEN);
    uLong adler_read = adler32(0L, read_buf, (uInt)ORIG_LEN);
    gzclose(gf_r);
    free(orig);
    free(read_buf);
    (void)ver;
    (void)read_bytes;
    (void)validate_ret;
    (void)adler_orig;
    (void)adler_read;
    // API sequence test completed successfully
    return 66;
}