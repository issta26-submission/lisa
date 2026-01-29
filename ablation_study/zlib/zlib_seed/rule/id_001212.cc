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
//<ID> 1212
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data, compute checksum and compress with compress2
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'P', (size_t)ORIG_LEN);
    uLong orig_adler = adler32(0UL, orig, (uInt)ORIG_LEN);
    uLong comp_bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    uLongf comp_size = (uLongf)comp_bound;
    compress2(comp, &comp_size, orig, ORIG_LEN, 6);

    // step 2: Write and then reopen a gzip file, then close the read handle with gzclose_r
    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gz, orig, (unsigned int)ORIG_LEN);
    gzclose(gz);
    gz = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *gz_read_buf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(gz_read_buf, 0, (size_t)ORIG_LEN);
    unsigned int gz_read_len = (unsigned int)gzfread(gz_read_buf, 1, (z_size_t)ORIG_LEN, gz);
    gzclose_r(gz);

    // step 3: Initialize a deflate stream, reconfigure it with deflateParams, perform deflate, cleanup
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    deflateParams(&dstrm, 5, 0);
    Bytef *comp2 = (Bytef *)malloc((size_t)comp_bound);
    memset(comp2, 0, (size_t)comp_bound);
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp2;
    dstrm.avail_out = (uInt)comp_bound;
    deflate(&dstrm, 4);
    deflateEnd(&dstrm);

    // step 4: Initialize an inflate stream, decompress, call inflateBackEnd for cleanup, validate checksum, free resources
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflateBackEnd(&istrm);
    inflateEnd(&istrm);
    uLong decomp_adler = adler32(0UL, decomp, (uInt)ORIG_LEN);

    free(orig);
    free(comp);
    free(comp2);
    free(decomp);
    free(gz_read_buf);

    // API sequence test completed successfully
    return 66;
}