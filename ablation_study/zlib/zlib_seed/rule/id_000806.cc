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
//<ID> 806
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare input buffer
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1024UL;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong crc_input = crc32(0L, input, (uInt)INPUT_LEN);

    // step 2: Initialize deflate stream, compute bound, compress via deflate, then end deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, INPUT_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)INPUT_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4); // Z_FINISH == 4
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflateBack stream, set a dictionary, then clean up inflateBack
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&istrm, 15, window, ver, (int)sizeof(z_stream));
    Bytef dict[32];
    memset(dict, 0x5A, sizeof(dict));
    inflateSetDictionary(&istrm, dict, (uInt)sizeof(dict));
    inflateBackEnd(&istrm);

    // step 4: Validate checksums, cleanup, and finish
    uLong crc_comp = crc32(0L, comp, (uInt)comp_size);
    uLong combined_crc = crc32_combine(crc_input, crc_comp, (off_t)comp_size);
    free(input);
    free(comp);
    (void)ver;
    (void)crc_input;
    (void)crc_comp;
    (void)combined_crc;
    // API sequence test completed successfully
    return 66;
}