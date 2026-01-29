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
//<ID> 423
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
    z_stream istrm;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    int deflate_init_ret = deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    int inflate_init_ret = inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    Bytef dictionary[] = "example_dictionary_for_zlib_sequence";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);
    Bytef input[] = "Sample payload to compress and then write via gzopen/gzwrite.";
    unsigned int input_len = (unsigned int)(sizeof(input) - 1);

    // step 2: Configure
    int def_set_ret = deflateSetDictionary(&dstrm, dictionary, dict_len);
    int inf_set_ret = inflateSetDictionary(&istrm, dictionary, dict_len);

    // step 3: Operate
    uLong bound = deflateBound(&dstrm, (uLong)input_len);
    Bytef comp_buf[512];
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)input_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)sizeof(comp_buf);
    int deflate_ret = deflate(&dstrm, 4); /* 4 ~ Z_FINISH */
    unsigned int comp_size = (unsigned int)(dstrm.total_out);
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gz, (voidpc)comp_buf, comp_size);
    int gzclose_ret = gzclose(gz);

    // step 4: Validate / Cleanup
    int deflate_end_ret = deflateEnd(&dstrm);
    int inflate_end_ret = inflateEnd(&istrm);
    (void)ver;
    (void)deflate_init_ret;
    (void)inflate_init_ret;
    (void)dict_len;
    (void)input_len;
    (void)def_set_ret;
    (void)inf_set_ret;
    (void)bound;
    (void)deflate_ret;
    (void)comp_size;
    (void)filename;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    (void)inflate_end_ret;
    // API sequence test completed successfully
    return 66;
}