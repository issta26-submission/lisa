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
//<ID> 655
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
    gzFile gz_file = Z_NULL;
    const char* filename = "zlib_seq_test_file.gz";

    // Buffers for compression/decompression
    Bytef uncomp_src_buf[100];
    Bytef comp_buf[200]; // Should be enough for compressed data
    Bytef uncomp_dest_buf[100];

    uLongf comp_len = sizeof(comp_buf);
    uLongf uncomp_dest_len_for_uncomp2 = sizeof(uncomp_dest_buf);
    uLong source_len_for_uncomp2 = sizeof(uncomp_src_buf); // For uncompress2 original source length

    // Buffers for gzread and checksums
    Bytef gz_read_buf[50];
    Bytef adler_buf[30];
    Bytef adler_z_buf[40];

    // Checksum variables
    uLong initial_adler = adler32(0L, Z_NULL, 0); // Initialize adler32 with no buffer, length 0
    uLong initial_adler_z = adler32_z(0L, Z_NULL, 0); // Initialize adler32_z with no buffer, length 0
    uLong crc_val1 = 0x11223344L;
    uLong crc_val2 = 0xAABBCCDDL;
    off_t combine_offset = 500; // For crc32_combine

    // String for gzputs
    const char* test_string = "This is a test string for gzputs.";

    int ret; // Generic return value for zlib functions

    // Step 2: Setup - Stream & File Initialization
    // Initialize deflate stream (for deflateEnd)
    memset(&def_strm, 0, sizeof(def_strm));
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    ret = deflateInit_(&def_strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, (int)sizeof(z_stream));

    // Initialize inflate stream (for inflateUndermine and inflateEnd)
    memset(&inf_strm, 0, sizeof(inf_strm));
    inf_strm.zalloc = Z_NULL;
    inf_strm.zfree = Z_NULL;
    inf_strm.opaque = Z_NULL;
    ret = inflateInit_(&inf_strm, ZLIB_VERSION, (int)sizeof(z_stream));

    // Prepare data buffers
    memset(uncomp_src_buf, 'S', sizeof(uncomp_src_buf));
    memset(comp_buf, 0, sizeof(comp_buf)); // Clear compressed buffer
    memset(uncomp_dest_buf, 0, sizeof(uncomp_dest_buf)); // Clear decompression destination
    memset(gz_read_buf, 0, sizeof(gz_read_buf));
    memset(adler_buf, 'A', sizeof(adler_buf));
    memset(adler_z_buf, 'Z', sizeof(adler_z_buf));

    // Open gzFile for writing
    gz_file = gzopen(filename, "wb");

    // Generate compressed data using `compress` for `uncompress2`'s source
    // `compress` is an available API and necessary to create valid input for `uncompress2`.
    ret = compress(comp_buf, &comp_len, uncomp_src_buf, source_len_for_uncomp2);

    // Step 3: GZFile Write Operations
    ret = gzputs(gz_file, test_string);
    ret = gzflush(gz_file, Z_SYNC_FLUSH); // Flush data to file
    ret = gzclose(gz_file); // Close after writing

    // Step 4: GZFile Read & uncompress2 Operations
    gz_file = gzopen(filename, "rb"); // Reopen file for reading
    ret = gzread(gz_file, gz_read_buf, (uInt)sizeof(gz_read_buf)); // Read data from the file

    // Call uncompress2 with valid data
    uncomp_dest_len_for_uncomp2 = sizeof(uncomp_dest_buf); // Reset dest_len for uncompress2
    uLong actual_comp_len_for_uncomp2 = comp_len; // Use the actual length from compress
    ret = uncompress2(uncomp_dest_buf, &uncomp_dest_len_for_uncomp2, comp_buf, &actual_comp_len_for_uncomp2);

    // Edge Case: uncompress2 with zero source length
    uLong zero_source_len = 0;
    uLongf zero_uncomp_dest_len = sizeof(uncomp_dest_buf);
    // A non-NULL source pointer is required, even if length is zero.
    // Passing comp_buf is fine here; uncompress2 should handle 0 length gracefully.
    ret = uncompress2(uncomp_dest_buf, &zero_uncomp_dest_len, comp_buf, &zero_source_len);

    ret = gzclose(gz_file); // Close file after reading

    // Step 5: Checksum & Inflate Operations
    // Calculate Adler-32 checksum (uInt len)
    initial_adler = adler32(initial_adler, adler_buf, (uInt)sizeof(adler_buf));

    // Edge Case: adler32 with Z_NULL buffer and non-zero length
    // As per zlib docs, if buf is Z_NULL and len > 0, returns the current adler value.
    initial_adler = adler32(initial_adler, Z_NULL, 10);

    // Calculate Adler-32 checksum (z_size_t len)
    initial_adler_z = adler32_z(initial_adler_z, adler_z_buf, (z_size_t)sizeof(adler_z_buf));

    // Combine CRC-32 checksums
    uLong combined_crc = crc32_combine(crc_val1, crc_val2, combine_offset);

    // Undermine the inflate stream (enabling undermining for testing purposes)
    ret = inflateUndermine(&inf_strm, 1);

    // Step 6: Cleanup
    ret = deflateEnd(&def_strm); // Clean up deflate stream
    ret = inflateEnd(&inf_strm); // Clean up inflate stream
    remove(filename); // Delete the temporary file

    printf("API sequence test completed successfully\n");

    return 66;
}