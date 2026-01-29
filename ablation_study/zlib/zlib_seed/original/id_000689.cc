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
//<ID> 689
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Step 1: Declarations
    z_stream def_strm;
    z_stream inf_strm;
    gz_header gz_hdr_out;
    gz_header gz_hdr_in;
    gzFile gz_file_w = Z_NULL;
    gzFile gz_file_r = Z_NULL;
    const char* filename = "zlib_api_test.gz";

    Bytef source_buf[256];
    Bytef compressed_buf[512]; // Buffer for compressed data (should be larger than source)
    Bytef uncompressed_buf[256]; // Buffer for uncompressed data

    uLongf compressed_len_actual; // Actual length of data after deflate
    uLongf uncompressed_len_actual; // Actual length of data after uncompress
    uLong source_len;

    uLong crc_val;
    int ret; // Generic return value for zlib functions
    uLong compile_flags;
    unsigned long codes_used;

    // Step 2: Setup - Initializations
    memset(&def_strm, 0, sizeof(def_strm));
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    ret = deflateInit2_(&def_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY, ZLIB_VERSION, (int)sizeof(z_stream));

    memset(&inf_strm, 0, sizeof(inf_strm));
    inf_strm.zalloc = Z_NULL;
    inf_strm.zfree = Z_NULL;
    inf_strm.opaque = Z_NULL;
    ret = inflateInit2_(&inf_strm, 15 + 32, ZLIB_VERSION, (int)sizeof(z_stream)); // 15+32 for gzip autodetection

    memset(&gz_hdr_out, 0, sizeof(gz_hdr_out));
    gz_hdr_out.text = 1;
    gz_hdr_out.time = 123456789;
    gz_hdr_out.os = 3; // Unix OS

    memset(&gz_hdr_in, 0, sizeof(gz_hdr_in)); // Initialize for receiving header info

    memset(source_buf, 'Z', sizeof(source_buf));
    source_len = sizeof(source_buf);

    memset(compressed_buf, 0, sizeof(compressed_buf));
    memset(uncompressed_buf, 0, sizeof(uncompressed_buf));

    crc_val = 0L; // Initial CRC value

    // Step 3: Deflate Stream Configuration and Compression
    ret = deflateSetHeader(&def_strm, &gz_hdr_out);

    def_strm.avail_in = (uInt)source_len;
    def_strm.next_in = source_buf;
    def_strm.avail_out = (uInt)sizeof(compressed_buf);
    def_strm.next_out = compressed_buf;
    ret = deflate(&def_strm, Z_FINISH); // Compress all data
    compressed_len_actual = sizeof(compressed_buf) - def_strm.avail_out; // Get actual compressed length

    // Step 4: Inflate Stream Operations and Checksums
    inf_strm.avail_in = (uInt)compressed_len_actual;
    inf_strm.next_in = compressed_buf;
    inf_strm.avail_out = (uInt)sizeof(uncompressed_buf);
    inf_strm.next_out = uncompressed_buf;

    ret = inflateGetHeader(&inf_strm, &gz_hdr_in); // Attempt to get header from the compressed data

    ret = inflate(&inf_strm, Z_FINISH); // Decompress all data
    codes_used = inflateCodesUsed(&inf_strm); // Get codes used after inflation

    compile_flags = zlibCompileFlags(); // Retrieve zlib compile flags

    crc_val = crc32(crc_val, source_buf, (uInt)source_len); // Calculate CRC32 of source data

    // Step 5: gzFile Operations and Uncompress (with Edge Case)
    gz_file_w = gzopen(filename, "wb"); // Open file for writing in binary gzip mode
    ret = gzwrite(gz_file_w, source_buf, (unsigned int)source_len); // Write source data to the gzFile
    ret = gzclose(gz_file_w); // Close the write file

    gz_file_r = gzopen(filename, "rb"); // Reopen file for reading
    ret = gzread(gz_file_r, uncompressed_buf, (unsigned int)sizeof(uncompressed_buf)); // Read data from gzFile
    ret = gzclose(gz_file_r); // Close the read file

    uncompressed_len_actual = sizeof(uncompressed_buf); // Reset for uncompress destination buffer size
    // Decompress data using the simple `uncompress` API
    ret = uncompress(uncompressed_buf, &uncompressed_len_actual, compressed_buf, compressed_len_actual);

    // Edge Case: Call uncompress with zero source length.
    uLongf zero_dest_len = sizeof(uncompressed_buf); // Output buffer size
    uLong zero_source_len = 0; // Zero input length
    Bytef dummy_source_ptr[1]; // A valid non-NULL pointer is required for source, even if length is 0
    // Ensure dummy_source_ptr is initialized to avoid warnings, though its content is irrelevant for len=0
    memset(dummy_source_ptr, 0, sizeof(dummy_source_ptr));
    ret = uncompress(uncompressed_buf, &zero_dest_len, dummy_source_ptr, zero_source_len);

    // Edge Case: Call inflateReset2 with an invalid windowBits value.
    // -1 is outside the valid range (8..15 or their negatives, or +16/+32).
    // This should typically return Z_STREAM_ERROR.
    ret = inflateReset2(&inf_strm, -1);

    // Step 6: Cleanup
    ret = deflateEnd(&def_strm); // End deflate stream to free resources
    ret = inflateEnd(&inf_strm); // End inflate stream to free resources

    remove(filename); // Delete the temporary file created

    printf("API sequence test completed successfully\n");

    return 66;
}