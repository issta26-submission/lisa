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
//<ID> 1060
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / Prepare source and compression stream
    const char *version = zlibVersion();
    const Bytef source[] = "zlib sample payload for compression -> write to gz -> seek -> inflate -> checksum combine";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int dinit_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)source_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)bound;
    int def_ret = deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    int dend_ret = deflateEnd(&dstrm);

    // step 2: Write compressed data to a gz file and perform a seek
    gzFile gzf = gzopen("tmp_zlib_sequence.gz", "wb");
    int gzwrite_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_size);
    off_t cur_offset = gzseek(gzf, 0, 1);
    int gzclose_ret_w = gzclose(gzf);

    // step 3: Initialize inflate stream, allow undermine, and decompress back into memory
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int iinit_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int iund_ret = inflateUndermine(&istrm, 1);
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_size;
    uLong out_bound = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (size_t)out_bound);
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)out_bound;
    int inf_ret = inflate(&istrm, 4);
    uLong out_size = istrm.total_out;
    int iend_ret = inflateEnd(&istrm);

    // step 4: Validate checksums, combine them, cleanup resources
    uLong crc_orig = crc32(0UL, source, (uInt)source_len);
    uLong crc_out = crc32(0UL, out_buf, (uInt)out_size);
    uLong crc_combined = crc32_combine(crc_orig, crc_out, (off_t)out_size);
    free(comp_buf);
    free(out_buf);
    (void)dinit_ret;
    (void)bound;
    (void)def_ret;
    (void)dend_ret;
    (void)gzwrite_ret;
    (void)cur_offset;
    (void)gzclose_ret_w;
    (void)iinit_ret;
    (void)iund_ret;
    (void)inf_ret;
    (void)iend_ret;
    (void)crc_combined;
    (void)version;
    // API sequence test completed successfully
    return 66;
}