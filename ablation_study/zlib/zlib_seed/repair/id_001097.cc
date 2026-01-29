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
//<ID> 1097
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it into an in-memory buffer
    const Bytef source[] = "zlib API sequence payload used to exercise compress2, inflate, and gz* helpers.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(source_len);
    uLongf comp_len = (uLongf)bound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_len);
    memset(comp_buf, 0, (size_t)comp_len);
    int comp_ret = compress2(comp_buf, &comp_len, source, source_len, 6);

    // step 2: Persist compressed bytes to a .gz file (write), then close the writer
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gwrote = gzwrite(gw, (voidpc)comp_buf, (unsigned int)comp_len);
    int gwclose = gzclose(gw);

    // step 3: Re-open the .gz file for reading, consume a byte via gzgetc_, then close using gzclose_r
    gzFile gr = gzopen("tmp_zlib_api_seq.gz", "rb");
    int first_byte = gzgetc_(gr);
    int grclose_r = gzclose_r(gr);

    // step 4: Inflate the in-memory compressed buffer, create a deflate stream and end it, then cleanup
    z_stream infstrm;
    memset(&infstrm, 0, (int)sizeof(z_stream));
    Bytef *decomp_buf = (Bytef *)malloc((size_t)source_len + 1);
    memset(decomp_buf, 0, (size_t)source_len + 1);
    infstrm.next_in = comp_buf;
    infstrm.avail_in = (uInt)comp_len;
    infstrm.next_out = decomp_buf;
    infstrm.avail_out = (uInt)source_len;
    int inf_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    int inf_ret = inflate(&infstrm, 0);
    int inf_end = inflateEnd(&infstrm);

    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(z_stream));
    int def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int def_end = deflateEnd(&defstrm);

    free(comp_buf);
    free(decomp_buf);

    (void)comp_ret;
    (void)bound;
    (void)comp_len;
    (void)gwrote;
    (void)gwclose;
    (void)first_byte;
    (void)grclose_r;
    (void)inf_init;
    (void)inf_ret;
    (void)inf_end;
    (void)def_init;
    (void)def_end;
    (void)version;
    (void)source_len;

    // API sequence test completed successfully
    return 66;
}