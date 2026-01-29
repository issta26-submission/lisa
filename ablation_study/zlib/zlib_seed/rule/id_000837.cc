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
//<ID> 837
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 128UL;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'X', (size_t)INPUT_LEN);
    gzFile gz = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzputc(gz, 'H');
    gzputc(gz, 'i');
    gzputc(gz, '\n');
    gzclose(gz);

    // step 2: Initialize and run deflate on the prepared buffer
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)INPUT_LEN;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong bound_est = deflateBound(&dstrm, INPUT_LEN);
    uInt compBufLen = (uInt)(bound_est + 16UL);
    Bytef *comp = (Bytef *)malloc((size_t)compBufLen);
    memset(comp, 0, (size_t)compBufLen);
    dstrm.next_out = comp;
    dstrm.avail_out = compBufLen;
    deflate(&dstrm, 4);
    uLong compressed_bytes = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflate, copy its state, and validate copy
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
    inflateEnd(&istrm_copy);
    inflateEnd(&istrm);

    // step 4: Cleanup
    free(input);
    free(comp);
    (void)ver;
    (void)bound_est;
    (void)compressed_bytes;
    // API sequence test completed successfully
    return 66;
}