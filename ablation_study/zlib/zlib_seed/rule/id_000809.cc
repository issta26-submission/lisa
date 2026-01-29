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
//<ID> 809
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 512;
    const uInt DICT_LEN = 32;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    Bytef *dict = (Bytef *)malloc((size_t)DICT_LEN);
    memset(input, 'X', (size_t)INPUT_LEN);
    memset(dict, 'D', (size_t)DICT_LEN);

    // step 2: Initialize deflate and inflate streams
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));

    // step 3: Configure dictionaries, compute bounds and combined CRC
    deflateSetDictionary(&dstrm, dict, (uInt)DICT_LEN);
    inflateSetDictionary(&istrm, dict, (uInt)DICT_LEN);
    uLong bound = deflateBound(&dstrm, (uLong)INPUT_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLong crc_input = crc32(0UL, input, (uInt)INPUT_LEN);
    uLong crc_dict = crc32(0UL, dict, (uInt)DICT_LEN);
    uLong crc_combined = crc32_combine(crc_input, crc_dict, (off_t)DICT_LEN);

    // step 4: Cleanup streams and buffers
    deflateEnd(&dstrm);
    inflateBackEnd(&istrm);
    free(input);
    free(dict);
    free(comp);
    (void)ver;
    (void)bound;
    (void)crc_input;
    (void)crc_dict;
    (void)crc_combined;
    // API sequence test completed successfully
    return 66;
}