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
//<ID> 822
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "Example zlib data to compress";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    int init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure deflate and buffers
    defstrm.next_in = (Bytef *)sample;
    defstrm.avail_in = (uInt)sample_len;
    uLong out_bound = deflateBound(&defstrm, sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)out_bound;
    int reset_ret = deflateReset(&defstrm);

    // step 3: Operate (compress, write to gzFile, query position)
    int def_ret = deflate(&defstrm, 4);
    uLong comp_len = defstrm.total_out;
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    off_t pos = gztell(gzf);
    int gz_cl = gzclose(gzf);

    // step 4: Configure inflateBack, cleanup and validate
    z_stream instrm;
    memset(&instrm, 0, sizeof(instrm));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int ibinit_ret = inflateBackInit_(&instrm, 15, window, version, (int)sizeof(z_stream));
    int ibend_ret = inflateBackEnd(&instrm);
    int def_end = deflateEnd(&defstrm);
    free(comp_buf);
    free(window);
    (void)version;
    (void)init_ret;
    (void)out_bound;
    (void)reset_ret;
    (void)def_ret;
    (void)comp_len;
    (void)gz_wr;
    (void)pos;
    (void)gz_cl;
    (void)ibinit_ret;
    (void)ibend_ret;
    (void)def_end;
    // API sequence test completed successfully
    return 66;
}