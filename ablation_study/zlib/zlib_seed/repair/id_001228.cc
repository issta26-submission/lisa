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
//<ID> 1228
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "zlib API sequence payload for gz, deflate and crc table demonstration.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    memset(&istrm, 0, (int)sizeof(istrm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure deflate and perform compression
    uLong bound = deflateBound(&dstrm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int def_ret = deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    int def_end_ret = deflateEnd(&dstrm);

    // step 3: Persist compressed data to a gzFile and query CRC table
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gzwrite_ret = gzwrite(gf, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size));
    const z_crc_t *crc_table = get_crc_table();
    z_crc_t crc_sample = crc_table ? crc_table[0] : 0;
    int gzclose_ret = gzclose(gf);

    // step 4: Finalize inflate backend and cleanup
    int inflate_back_end_ret = inflateBackEnd(&istrm);
    free(comp_buf);
    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)gzwrite_ret;
    (void)crc_sample;
    (void)gzclose_ret;
    (void)version;
    (void)inflate_back_end_ret;
    // API sequence test completed successfully
    return 66;
}