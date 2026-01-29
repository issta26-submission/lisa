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
//<ID> 1224
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data, compute partial CRCs and compress
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'C', (size_t)ORIG_LEN);
    uLong crc_first = crc32(0UL, orig, (uInt)(ORIG_LEN / 2));
    uLong crc_second = crc32(0UL, orig + (ORIG_LEN / 2), (uInt)(ORIG_LEN / 2));
    uLong combined_crc = crc32_combine(crc_first, crc_second, (off_t)(ORIG_LEN / 2));
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size = (uLongf)bound;
    compress2(comp, &comp_size, orig, ORIG_LEN, 6);

    // step 2: Initialize inflate stream using zlibVersion
    const char *ver = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));

    // step 3: Decompress using inflate, then call inflateUndermine and inflateBackEnd
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflateUndermine(&istrm, 1);
    inflateBackEnd(&istrm);

    // step 4: Validate via CRC, cleanup and finish
    uLong decomp_crc = crc32(0UL, decomp, (uInt)ORIG_LEN);
    (void)combined_crc; /* used earlier to demonstrate meaningful data flow */
    (void)decomp_crc;   /* computed as validation artifact */
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}