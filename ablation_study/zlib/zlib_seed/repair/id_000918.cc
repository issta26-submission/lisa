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
//<ID> 918
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef input[] = "Hello zlib test payload";
    uLong input_len = (uLong)(sizeof(input) - 1);
    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    int init_def = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    defstrm.next_in = (Bytef *)input;
    defstrm.avail_in = (uInt)input_len;
    uLong bound = deflateBound(&defstrm, input_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&defstrm, 4);
    uLong comp_len = defstrm.total_out;
    int def_end = deflateEnd(&defstrm);
    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_put_ret = gzputs(gzf, (const char *)input);
    off_t offset = gzoffset(gzf);
    int gz_cl = gzclose(gzf);

    // step 4: Validate / Cleanup
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc = crc_tbl[0];
    z_stream infstrm;
    memset(&infstrm, 0, sizeof(infstrm));
    int init_inf = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    infstrm.next_in = comp_buf;
    infstrm.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(input_len + 16));
    memset(decomp_buf, 0, (size_t)(input_len + 16));
    infstrm.next_out = decomp_buf;
    infstrm.avail_out = (uInt)(input_len + 16);
    int inf_ret = inflate(&infstrm, 4);
    int inf_end = inflateEnd(&infstrm);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)init_def;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end;
    (void)gz_put_ret;
    (void)offset;
    (void)gz_cl;
    (void)crc_tbl;
    (void)first_crc;
    (void)init_inf;
    (void)inf_ret;
    (void)inf_end;
    // API sequence test completed successfully
    return 66;
}