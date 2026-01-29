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
//<ID> 922
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 1024UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp_def = (Bytef *)malloc((size_t)bound);
    memset(comp_def, 0, (size_t)bound);
    Bytef *decomp_def = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp_def, 0, (size_t)ORIG_LEN);
    Bytef *comp_z = (Bytef *)malloc((size_t)bound);
    memset(comp_z, 0, (size_t)bound);
    Bytef *decomp_z = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp_z, 0, (size_t)ORIG_LEN);

    // step 2: Initialize and run deflate to produce compressed data (zlib format)
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp_def;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong comp_def_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflate and decompress the deflate-produced buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp_def;
    istrm.avail_in = (uInt)comp_def_size;
    istrm.next_out = decomp_def;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflateEnd(&istrm);

    // step 4: Use compress2 / uncompress2 for a second zlib flow, then cleanup
    uLongf comp_z_len = (uLongf)bound;
    compress2(comp_z, &comp_z_len, orig, ORIG_LEN, 6);
    uLong sourceLen = (uLong)comp_z_len;
    uLongf decomp_z_len = (uLongf)ORIG_LEN;
    uncompress2(decomp_z, &decomp_z_len, comp_z, &sourceLen);

    free(orig);
    free(comp_def);
    free(decomp_def);
    free(comp_z);
    free(decomp_z);
    (void)ver;
    (void)bound;
    (void)comp_def_size;
    (void)comp_z_len;
    (void)decomp_z_len;
    // API sequence test completed successfully
    return 66;
}