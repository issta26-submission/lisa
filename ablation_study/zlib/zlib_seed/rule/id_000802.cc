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
//<ID> 802
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare version, source and dictionary buffers
    const char *ver = zlibVersion();
    const uLong SOURCE_LEN = 1500UL;
    Bytef *source = (Bytef *)malloc((size_t)SOURCE_LEN);
    memset(source, 'X', (size_t)SOURCE_LEN);
    const uInt DICT_LEN = 64U;
    Bytef *dictionary = (Bytef *)malloc((size_t)DICT_LEN);
    memset(dictionary, 'D', (size_t)DICT_LEN);

    // step 2: Initialize a deflate stream, compute bound, and finalize it
    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    defstrm.zalloc = NULL;
    defstrm.zfree = NULL;
    defstrm.opaque = NULL;
    deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&defstrm, SOURCE_LEN);
    Bytef *compbuf = (Bytef *)malloc((size_t)bound);
    memset(compbuf, 0, (size_t)bound);
    deflateEnd(&defstrm);

    // step 3: Initialize a normal inflate stream, set a dictionary, validate via CRC combine
    z_stream infstrm;
    memset(&infstrm, 0, sizeof(infstrm));
    infstrm.zalloc = NULL;
    infstrm.zfree = NULL;
    infstrm.opaque = NULL;
    inflateInit_(&infstrm, ver, (int)sizeof(z_stream));
    inflateSetDictionary(&infstrm, dictionary, DICT_LEN);
    uLong crc_src = crc32(0L, source, (uInt)SOURCE_LEN);
    uLong crc_dict = crc32(0L, dictionary, (uInt)DICT_LEN);
    uLong crc_combined = crc32_combine(crc_src, crc_dict, (off_t)DICT_LEN);

    // step 4: Initialize inflateBack, teardown, and free resources
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    backstrm.zalloc = NULL;
    backstrm.zfree = NULL;
    backstrm.opaque = NULL;
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&backstrm, 15, window, ver, (int)sizeof(z_stream));
    inflateBackEnd(&backstrm);
    inflateEnd(&infstrm);
    free(source);
    free(dictionary);
    free(compbuf);
    (void)bound;
    (void)crc_combined;
    // API sequence test completed successfully
    return 66;
}