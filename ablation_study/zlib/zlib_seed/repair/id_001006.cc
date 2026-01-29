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
//<ID> 1006
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence: produce compressed data, copy inflate stream, set dictionary, write and read via gzOpen/gzgetc.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    int dinit_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    (void)dinit_ret;

    // step 2: Configure
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    defstrm.next_in = (Bytef *)source;
    defstrm.avail_in = (uInt)source_len;
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&defstrm, 0);
    uLong comp_size = defstrm.total_out;
    int def_end_ret = deflateEnd(&defstrm);
    (void)def_ret;
    (void)def_end_ret;

    z_stream infstrm;
    memset(&infstrm, 0, sizeof(infstrm));
    int iinit_ret = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    infstrm.next_in = comp_buf;
    infstrm.avail_in = (uInt)comp_size;
    (void)iinit_ret;

    z_stream infcopy;
    memset(&infcopy, 0, sizeof(infcopy));
    int copy_ret = inflateCopy(&infcopy, &infstrm);
    (void)copy_ret;

    Bytef dict_buf[16];
    memset(dict_buf, 'D', sizeof(dict_buf));
    uInt dict_len = (uInt)sizeof(dict_buf);
    int setdict_ret = inflateSetDictionary(&infcopy, dict_buf, dict_len);
    (void)setdict_ret;

    // step 4: Validate / Cleanup
    gzFile gzf_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_written = gzwrite(gzf_w, comp_buf, (unsigned int)comp_size);
    int gzclose_w = gzclose(gzf_w);
    (void)gz_written;
    (void)gzclose_w;

    gzFile gzf_r = gzopen("test_zlib_api_sequence.gz", "rb");
    int first_char = gzgetc(gzf_r);
    int gzclose_r = gzclose(gzf_r);
    (void)first_char;
    (void)gzclose_r;

    int inf_end_ret = inflateEnd(&infstrm);
    int infcopy_end_ret = inflateEnd(&infcopy);
    free(comp_buf);
    (void)version;
    (void)source_len;
    (void)bound;
    (void)comp_size;
    (void)inf_end_ret;
    (void)infcopy_end_ret;

    return 66;
    // API sequence test completed successfully
}