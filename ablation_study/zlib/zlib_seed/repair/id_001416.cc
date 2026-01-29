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
//<ID> 1416
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence test: write gz -> seek64 -> rewind -> read -> inflate/get header -> deflateReset";
    uLong src_len = (uLong)(sizeof(src) - 1);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gz, src, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gzclose_ret1 = gzclose(gz);

    // step 2: Configure
    gz = gzopen("test_zlib_api_sequence.gz", "rb");
    off64_t seek_pos = gzseek64(gz, 0, 0);
    int grewind = gzrewind(gz);
    Bytef *read_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(read_buf, 0, (int)(src_len + 16));
    int gr = gzread(gz, (voidp)read_buf, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));

    // step 3: Operate
    const char *version = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (int)sizeof(head));
    int iget_ret = inflateGetHeader(&istrm, &head);
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(out_buf, 0, (int)(src_len + 64));
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)((src_len + 64) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(src_len + 64));
    int inf_ret = inflate(&istrm, 0);

    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int dreset_ret = deflateReset(&dstrm);

    // step 4: Validate & Cleanup
    (void)gw;
    (void)gzclose_ret1;
    (void)seek_pos;
    (void)grewind;
    (void)gr;
    (void)inf_init_ret;
    (void)iget_ret;
    (void)inf_ret;
    (void)def_init_ret;
    (void)dreset_ret;
    int inf_end_ret = inflateEnd(&istrm);
    int def_end_ret = deflateEnd(&dstrm);
    int gzclose_ret2 = gzclose(gz);
    free(read_buf);
    free(out_buf);
    return 66;
    // API sequence test completed successfully
}