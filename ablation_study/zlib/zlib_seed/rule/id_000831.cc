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
//<ID> 831
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong INPUT_LEN = 512UL;
    const char *ver = zlibVersion();
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    gzFile gz_init = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzputc(gz_init, 'H');
    gzclose(gz_init);

    // step 2: Initialize deflate stream and compute bound
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, INPUT_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)INPUT_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;

    // step 3: Operate: deflate and prepare inflate streams (with a copy)
    deflate(&dstrm, 4);
    uLong compressed_bytes = dstrm.total_out;
    deflateEnd(&dstrm);
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compressed_bytes;
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    inflateCopy(&istrm_copy, &istrm);

    // step 4: Validate (write a small marker) & Cleanup
    gzFile gz_app = gzopen("tmp_zlib_api_seq.gz", "ab");
    gzputc(gz_app, (int)(compressed_bytes & 0xFF));
    gzclose(gz_app);
    inflateEnd(&istrm);
    inflateEnd(&istrm_copy);
    free(input);
    free(comp);
    (void)bound;
    (void)compressed_bytes;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}