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
//<ID> 1233
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare original data and compute CRC
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    uLong orig_crc = crc32_z(0UL, orig, (z_size_t)ORIG_LEN);

    // step 2: Compress the original data
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf comp_len = (uLongf)bound;
    compress2(comp, &comp_len, orig, ORIG_LEN, 6);

    // step 3: Initialize inflate stream, prime bits, set a dictionary, and inflate
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    const char *ver = zlibVersion();
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflatePrime(&istrm, 1, 1);
    const uInt DICT_LEN = 16U;
    Bytef *dict = (Bytef *)malloc((size_t)DICT_LEN);
    memset(dict, 'D', (size_t)DICT_LEN);
    inflateSetDictionary(&istrm, dict, DICT_LEN);
    inflate(&istrm, 0);
    uLong decomp_crc = crc32_z(0UL, decomp, (z_size_t)ORIG_LEN);

    // step 4: Cleanup
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);
    free(dict);
    // API sequence test completed successfully
    return 66;
}