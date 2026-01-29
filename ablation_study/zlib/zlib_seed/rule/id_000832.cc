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
//<ID> 832
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 64UL;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);

    // step 2: Initialize and configure deflate stream
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)INPUT_LEN;
    uLong bound = deflateBound(&dstrm, INPUT_LEN);
    uInt COMP_LEN = (uInt)(bound + 16UL);
    Bytef *comp = (Bytef *)malloc((size_t)COMP_LEN);
    memset(comp, 0, (size_t)COMP_LEN);
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)COMP_LEN;

    // step 3: Operate (deflate) -> Validate (inflate copy) -> Gzip putc test
    deflate(&dstrm, 4); /* Z_FINISH */
    uLong compressed_bytes = dstrm.total_out;
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
    gzFile gz = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzputc(gz, 'Z');
    gzclose(gz);

    // step 4: Cleanup
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    inflateEnd(&istrm_copy);
    free(input);
    free(comp);

    // API sequence test completed successfully
    (void)ver;
    (void)compressed_bytes;
    return 66;
}