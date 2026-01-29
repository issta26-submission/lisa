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
//<ID> 1220
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "zlib API sequence payload for deflate / inflateBack / gz operations.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    z_stream back_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&back_strm, 0, (int)sizeof(back_strm));

    // step 2: Initialize deflate stream and perform compression
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Initialize inflate-backend (inflateBack) structures and tear down
    int back_init_ret = inflateBackInit_(&back_strm, 15, (unsigned char *)NULL, version, (int)sizeof(z_stream));
    int back_end_ret = inflateBackEnd(&back_strm);

    // step 4: Write compressed output to a gzip file, query CRC table and cleanup
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gzwrite_ret = 0;
    if (gf) gzwrite_ret = gzwrite(gf, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size));
    const z_crc_t *crc_tab = get_crc_table();
    z_crc_t crc_first = crc_tab ? crc_tab[0] : 0;
    int gzclose_ret = gzclose(gf);
    free(comp_buf);
    (void)version;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)back_init_ret;
    (void)back_end_ret;
    (void)gzwrite_ret;
    (void)crc_first;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}