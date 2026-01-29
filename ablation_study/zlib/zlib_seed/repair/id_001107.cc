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
//<ID> 1107
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source payload and lengths
    const Bytef source[] = "zlib fuzz payload for deflate + crc32 + inflateBackEnd cleanup.";
    uLong source_len = (uLong)(sizeof(source) - 1);

    // step 2: Configure deflate stream and allocate output buffer
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)source_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)bound;

    // step 3: Operate - compress the data
    int def_ret = deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    int def_end_ret = deflateEnd(&dstrm);

    // step 4: Validate checksum and cleanup resources
    uLong crc = crc32(0UL, comp_buf, (uInt)comp_size);
    z_stream back_strm;
    memset(&back_strm, 0, (int)sizeof(back_strm));
    int back_end_ret = inflateBackEnd(&back_strm);
    free(comp_buf);
    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)def_end_ret;
    (void)crc;
    (void)back_end_ret;
    return 66;
    // API sequence test completed successfully
}