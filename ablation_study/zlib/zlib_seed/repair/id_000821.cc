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
//<ID> 821
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "Zlib API sequence sample data";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    z_stream def;
    memset(&def, 0, sizeof(def));
    int init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    def.next_in = (Bytef *)sample;
    def.avail_in = (uInt)sample_len;
    uLong bound = deflateBound(&def, sample_len);
    Bytef *comp1 = (Bytef *)malloc((size_t)bound);
    memset(comp1, 0, (size_t)bound);
    def.next_out = comp1;
    def.avail_out = (uInt)bound;
    int def_ret1 = deflate(&def, 4);
    uLong comp1_len = def.total_out;
    int reset_ret = deflateReset(&def);
    Bytef *comp2 = (Bytef *)malloc((size_t)bound);
    memset(comp2, 0, (size_t)bound);
    def.next_in = (Bytef *)sample;
    def.avail_in = (uInt)sample_len;
    def.next_out = comp2;
    def.avail_out = (uInt)bound;
    int def_ret2 = deflate(&def, 4);
    uLong comp2_len = def.total_out;
    z_stream backStrm;
    memset(&backStrm, 0, sizeof(backStrm));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int ibinit_ret = inflateBackInit_(&backStrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf_w = gzopen(fname, "wb");
    int gz_wr1 = gzwrite(gzf_w, comp1, (unsigned int)comp1_len);
    int gz_wr2 = gzwrite(gzf_w, comp2, (unsigned int)comp2_len);
    off_t pos_after_writes = gztell(gzf_w);
    int gz_cl_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen(fname, "rb");
    Bytef *read_back = (Bytef *)malloc((size_t)(sample_len + 16));
    memset(read_back, 0, (size_t)(sample_len + 16));
    int gz_rd = gzread(gzf_r, read_back, (unsigned int)(sample_len + 16));
    int gz_cl_r = gzclose(gzf_r);

    // step 4: Validate / Cleanup
    int def_end = deflateEnd(&def);
    int ibend_ret = inflateBackEnd(&backStrm);
    free(comp1);
    free(comp2);
    free(read_back);
    (void)version;
    (void)init_ret;
    (void)bound;
    (void)def_ret1;
    (void)comp1_len;
    (void)reset_ret;
    (void)def_ret2;
    (void)comp2_len;
    (void)ibinit_ret;
    (void)fname;
    (void)gz_wr1;
    (void)gz_wr2;
    (void)pos_after_writes;
    (void)gz_cl_w;
    (void)gz_rd;
    (void)gz_cl_r;
    (void)def_end;
    (void)ibend_ret;
    // API sequence test completed successfully
    return 66;
}