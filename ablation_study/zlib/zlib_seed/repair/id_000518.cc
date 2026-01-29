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
//<ID> 518
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef input[256];
    memset(input, 'A', sizeof(input));
    uLong adler1 = adler32(1UL, input, (uInt)sizeof(input));
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    int prime_ret = deflatePrime(&def, 3, 5);

    // step 2: Configure streams and copy inflater
    Bytef compbuf[1024];
    memset(compbuf, 0, sizeof(compbuf));
    def.next_in = input;
    def.avail_in = (uInt)sizeof(input);
    def.next_out = compbuf;
    def.avail_out = (uInt)sizeof(compbuf);
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, ver, (int)sizeof(z_stream));
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    int copy_ret = inflateCopy(&inf_copy, &inf);

    // step 3: Operate (compress, write to gz, compute checksums)
    int def_ret = deflate(&def, 4); /* 4 == Z_FINISH */
    uLong produced = def.total_out;
    gzFile gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_ret = gzwrite(gzf, compbuf, (unsigned int)produced);
    off_t offset_after_write = gzoffset(gzf);
    int gclose_ret = gzclose(gzf);
    uLong adler2 = adler32(0UL, compbuf, (uInt)produced);
    uLong combined = adler32_combine64(adler1, adler2, (off64_t)produced);
    const z_crc_t *crc_table = get_crc_table();
    uLong mixed = combined ^ (uLong)crc_table[0];

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def);
    int inf_end_ret = inflateEnd(&inf);
    int inf_copy_end_ret = inflateEnd(&inf_copy);

    (void)ver;
    (void)adler1;
    (void)def_init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)produced;
    (void)gw_ret;
    (void)offset_after_write;
    (void)gclose_ret;
    (void)adler2;
    (void)combined;
    (void)crc_table;
    (void)mixed;
    (void)inf_init_ret;
    (void)copy_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)inf_copy_end_ret;
    // API sequence test completed successfully
    return 66;
}