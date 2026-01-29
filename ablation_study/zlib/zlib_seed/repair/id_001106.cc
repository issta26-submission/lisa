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
//<ID> 1106
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and deflate initialization
    const Bytef source[] = "zlib API sequence payload: initialize -> deflate -> crc -> inflateBackEnd -> cleanup.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    const char *version = zlibVersion();
    int dinit_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong comp_bound = deflateBound(&dstrm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);

    // step 2: Configure deflate stream and perform compression
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)source_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)comp_bound;
    int def_ret = deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;

    // step 3: Validate compressed output with CRC and prepare inflate stream for backend cleanup
    uLong comp_crc = crc32(0UL, comp_buf, (uInt)comp_size);
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int iinit_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_size;
    int iback_ret = inflateBackEnd(&istrm);

    // step 4: Cleanup resources
    int dend_ret = deflateEnd(&dstrm);
    int iend_ret = inflateEnd(&istrm);
    free(comp_buf);
    (void)version;
    (void)dinit_ret;
    (void)comp_bound;
    (void)def_ret;
    (void)comp_size;
    (void)comp_crc;
    (void)iinit_ret;
    (void)iback_ret;
    (void)dend_ret;
    (void)iend_ret;
    // API sequence test completed successfully
    return 66;
}