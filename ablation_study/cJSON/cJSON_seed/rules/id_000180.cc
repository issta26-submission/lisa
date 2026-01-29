#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 180
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(123.456);
    cJSON *str_item = cJSON_CreateString("example");
    size_t buf_len = 256;
    char *buffer = (char *)cJSON_malloc(buf_len);
    const char *version = NULL;
    double extracted_number = 0.0;
    cJSON_bool is_string = 0;
    cJSON_bool printed = 0;

    // step 2: Setup / Configure
    memset(buffer, 0, buf_len);
    cJSON_AddItemToObject(root, "num", num_item);
    cJSON_AddItemToObject(root, "msg", str_item);

    // step 3: Operate / Validate
    version = cJSON_Version();
    extracted_number = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "num"));
    is_string = cJSON_IsString(cJSON_GetObjectItem(root, "msg"));
    printed = cJSON_PrintPreallocated(root, buffer, (int)buf_len, 1);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)version;
    (void)extracted_number;
    (void)is_string;
    (void)printed;
    return 66;
}