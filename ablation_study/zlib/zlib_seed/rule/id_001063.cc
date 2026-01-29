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
//<ID> 1063
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    const Bytef dict_init[] = "preset_dictionary";
    const uInt DICT_LEN = (uInt)(sizeof(dict_init) - 1);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

    // step 2: Initialize deflate, set dictionary, compress
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    deflateSetDictionary(&dstrm, dict_init, DICT_LEN);
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflate, attempt inflate (will request dictionary), provide dictionary, continue inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    int res1 = inflate(&istrm, 0);
    inflateSetDictionary(&istrm, dict_init, DICT_LEN);
    int res2 = inflate(&istrm, 0);
    uLong decomp_size = istrm.total_out;
    inflateEnd(&istrm);

    // step 4: Write outputs with gzopen & gzdopen, cleanup resources
    gzFile gf = gzopen("zlib_out.gz", "wb");
    gzwrite(gf, decomp, (unsigned int)decomp_size);
    gzclose(gf);
    gzFile gout = gzdopen(1, "wb");
    gzwrite(gout, comp, (unsigned int)comp_size);
    gzclose(gout);

    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)res1;
    (void)res2;
    // API sequence test completed successfully
    return 66;
}