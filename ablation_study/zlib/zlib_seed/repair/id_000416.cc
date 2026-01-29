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
//<ID> 416
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    int deflate_init_ret = deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    Bytef dictionary[] = "zlib_test_dictionary";
    uInt dictionary_len = (uInt)(sizeof(dictionary) - 1);
    Bytef src[] = "payload_data_for_deflateBound_and_gzwrite";
    unsigned int src_len = (unsigned int)(sizeof(src) - 1);
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    const z_crc_t *crc_table = get_crc_table();

    // step 2: Configure
    int dict_set_ret = deflateSetDictionary(&dstrm, dictionary, dictionary_len);
    int gzbuffer_ret = gzbuffer(gz, 8192U);

    // step 3: Operate
    uLong bound = deflateBound(&dstrm, (uLong)src_len);
    Bytef * outbuf = new Bytef[(size_t)bound + 1];
    uInt avail_out_val = (uInt)((size_t)bound + 1);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)src_len;
    dstrm.next_out = outbuf;
    dstrm.avail_out = avail_out_val;
    Bytef * got_dict = new Bytef[(size_t)dictionary_len + 1];
    uInt got_len = dictionary_len;
    int dict_get_ret = deflateGetDictionary(&dstrm, got_dict, &got_len);
    int gzwrite_ret = gzwrite(gz, (voidpc)src, src_len);
    uLong crc_val = crc32(0UL, src, (uInt)src_len);

    // step 4: Validate / Cleanup
    int gzclose_ret = gzclose(gz);
    int deflate_end_ret = deflateEnd(&dstrm);
    delete [] outbuf;
    delete [] got_dict;
    (void)ver;
    (void)deflate_init_ret;
    (void)dict_set_ret;
    (void)gzbuffer_ret;
    (void)bound;
    (void)avail_out_val;
    (void)dict_get_ret;
    (void)gzwrite_ret;
    (void)crc_table;
    (void)crc_val;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}