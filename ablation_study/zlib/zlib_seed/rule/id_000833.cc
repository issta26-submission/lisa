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
//<ID> 833
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare input
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 256UL;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)INPUT_LEN;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, INPUT_LEN);

    // step 2: Configure output buffer and run deflate
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflate stream and make a copy of its state
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    inflateCopy(&istrm_copy, &istrm);
    inflateEnd(&istrm_copy);
    inflateEnd(&istrm);

    // step 4: Gzip file write, cleanup
    gzFile gz = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzputc(gz, 'Z');
    gzclose(gz);
    free(input);
    free(comp);
    // API sequence test completed successfully
    return 66;
}