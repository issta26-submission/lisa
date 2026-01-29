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
//<ID> 836
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
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

    // step 2: Initialize deflate, compute bound, perform deflate
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, INPUT_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    int deflate_ret = deflate(&dstrm, 4); /* Z_FINISH */
    uLong comp_size = dstrm.total_out;
    int deflate_end_ret = deflateEnd(&dstrm);

    // step 3: Initialize inflate, copy its state, and perform gz operations
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    z_stream copy_stream;
    memset(&copy_stream, 0, sizeof(copy_stream));
    int copy_ret = inflateCopy(&copy_stream, &istrm);
    gzFile gz = gzopen("test_zlib_api_seq.gz", "wb");
    int putc_ret = gzputc(gz, (int)input[0]);
    int gzclose_ret = gzclose(gz);

    // step 4: Cleanup and finalize
    int inflate_end_ret1 = inflateEnd(&istrm);
    int inflate_end_ret2 = inflateEnd(&copy_stream);
    free(input);
    free(comp);
    (void)ver;
    (void)bound;
    (void)deflate_ret;
    (void)deflate_end_ret;
    (void)comp_size;
    (void)copy_ret;
    (void)putc_ret;
    (void)gzclose_ret;
    (void)inflate_end_ret1;
    (void)inflate_end_ret2;
    // API sequence test completed successfully
    return 66;
}