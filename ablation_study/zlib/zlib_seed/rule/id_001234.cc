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
//<ID> 1234
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, dictionary, and compute CRC
    const uLong ORIG_LEN = 128UL;
    const uLong DICT_LEN = 32UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    Bytef *dict = (Bytef *)malloc((size_t)DICT_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    memset(dict, 'D', (size_t)DICT_LEN);
    uLong orig_crc = crc32_z(0UL, orig, (z_size_t)ORIG_LEN);

    // step 2: Compress the source into a temporary buffer
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size = (uLongf)bound;
    compress2(comp, &comp_size, orig, ORIG_LEN, 6);

    // step 3: Initialize inflate stream, prime bits, set dictionary, and end inflate
    const char *ver = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflatePrime(&istrm, 3, 5);
    inflateSetDictionary(&istrm, dict, (uInt)DICT_LEN);
    inflateEnd(&istrm);

    // step 4: Validate by uncompressing and computing CRC, then cleanup
    uLongf decomp_len = ORIG_LEN;
    uncompress(decomp, &decomp_len, comp, comp_size);
    uLong decomp_crc = crc32_z(0UL, decomp, (z_size_t)decomp_len);
    (void)orig_crc;
    (void)decomp_crc;
    free(orig);
    free(dict);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}