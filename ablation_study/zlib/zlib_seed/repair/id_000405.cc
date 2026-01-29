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
//<ID> 405
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
    Bytef dict[] = "example-dictionary-for-deflate";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    Bytef input[] = "Zlib API sequence payload for deflate -> gzwrite -> gzoffset64 test.";
    uLong input_len = (uLong)(sizeof(input) - 1);
    Bytef outbuf[512];
    uInt outbuf_size = (uInt)sizeof(outbuf);
    int init_ret = deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure
    int setdict_ret = deflateSetDictionary(&dstrm, dict, dictLen);
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)input_len;
    dstrm.next_out = outbuf;
    dstrm.avail_out = outbuf_size;

    // step 3: Operate
    int def_ret = deflate(&dstrm, 0);
    uLong produced = dstrm.total_out;
    unsigned int write_len = (unsigned int)produced;
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gzbuf_ret = gzbuffer(gz, 1024);
    int gzwrite_ret = gzwrite(gz, (voidpc)outbuf, write_len);
    off64_t offset64 = gzoffset64(gz);
    int eof_flag = gzeof(gz);
    int gzclose_ret = gzclose(gz);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&dstrm);
    (void)ver;
    (void)init_ret;
    (void)setdict_ret;
    (void)def_ret;
    (void)produced;
    (void)write_len;
    (void)gz;
    (void)gzbuf_ret;
    (void)gzwrite_ret;
    (void)offset64;
    (void)eof_flag;
    (void)gzclose_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}