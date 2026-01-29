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
//<ID> 1064
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    Bytef dict_buf[] = "example_dictionary";
    uInt dict_len = (uInt)(sizeof(dict_buf) - 1);

    // step 2: Initialize deflate and perform compression
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    int dinit = deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    int ddef = deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    int dend = deflateEnd(&dstrm);

    // step 3: Initialize inflate, set dictionary, and perform inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    int iinit = inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    int idict = inflateSetDictionary(&istrm, dict_buf, dict_len);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    int iret = inflate(&istrm, 0);
    int iend = inflateEnd(&istrm);

    // step 4: Write data to gzip files and cleanup
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gwritten = gzwrite(gf, decomp, (unsigned int)ORIG_LEN);
    int gclose_ret = gzclose(gf);
    gzFile gdf = gzdopen(1, "wb");
    int gdwritten = gzwrite(gdf, orig, (unsigned int)ORIG_LEN);
    int gdf_close = gzclose(gdf);
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)bound;
    (void)comp_size;
    (void)dinit;
    (void)ddef;
    (void)dend;
    (void)iinit;
    (void)idict;
    (void)iret;
    (void)iend;
    (void)gwritten;
    (void)gclose_ret;
    (void)gdwritten;
    (void)gdf_close;
    // API sequence test completed successfully
    return 66;
}