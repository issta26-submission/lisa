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
//<ID> 317
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "numbers");
    cJSON *num_a = cJSON_CreateNumber(3.14);
    cJSON *num_b = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num_a);
    cJSON_AddItemToArray(arr, num_b);
    cJSON *str_item = cJSON_CreateString("example_string");
    cJSON_AddItemToObject(root, "label", str_item);
    cJSON *raw_item = cJSON_CreateRaw("{\"x\":1}");
    cJSON_AddItemToObject(root, "raw_blob", raw_item);

    // step 2: Configure
    const char *extracted = cJSON_GetStringValue(str_item);
    (void)extracted;
    cJSON_bool is_raw = cJSON_IsRaw(raw_item);
    cJSON_AddBoolToObject(root, "raw_blob_is_raw", is_raw);
    cJSON_bool is_invalid = cJSON_IsInvalid(root);
    cJSON_AddBoolToObject(root, "root_is_invalid", is_invalid);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(512);
    memset(buffer, 0, 512);
    /* copy printed JSON into buffer (assume printed is non-NULL as produced by cJSON_PrintUnformatted) */
    memcpy(buffer, printed, strlen(printed));
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}