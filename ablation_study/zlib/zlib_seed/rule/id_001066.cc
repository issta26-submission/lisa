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
//<ID> 1066
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
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);

    // step 2: Initialize deflate and perform compression
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Write original and compressed data to gzip handles (gzopen & gzdopen) and flush
    gzFile gw1 = gzopen("test_zlib_api_sequence_out.gz", "wb");
    gzwrite(gw1, orig, (unsigned int)ORIG_LEN);
    gzclose(gw1);
    gzFile gw2 = gzdopen(1, "wb");
    gzwrite(gw2, comp, (unsigned int)comp_size);
    gzclose(gw2);

    // step 4: Initialize inflate, set a dictionary for validation, finalize and cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    inflateSetDictionary(&istrm, orig, (uInt)(ORIG_LEN / 2));
    inflateEnd(&istrm);

    free(orig);
    free(comp);
    (void)comp_size;
    // API sequence test completed successfully
    return 66;
}