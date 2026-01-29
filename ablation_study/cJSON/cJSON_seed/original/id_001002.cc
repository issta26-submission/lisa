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
//<ID> 1002
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"root\":{\"arr\":[10,20,false],\"value\":3.14},\"flag\":false}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *root_obj = cJSON_GetObjectItem(root, "root");
    cJSON *arr = cJSON_GetObjectItem(root_obj, "arr");
    cJSON *second_elem = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_number = cJSON_IsNumber(second_elem);
    cJSON *value_item = cJSON_GetObjectItem(root_obj, "value");
    cJSON_bool value_is_number = cJSON_IsNumber(value_item);
    cJSON *missing_item = cJSON_GetObjectItem(root_obj, "missing");
    cJSON_bool missing_invalid = cJSON_IsInvalid(missing_item);
    cJSON_AddTrueToObject(root, "added_flag");
    cJSON_AddBoolToObject(root, "second_was_number", second_is_number);
    cJSON_AddBoolToObject(root, "value_was_number", value_is_number);
    cJSON_AddBoolToObject(root, "missing_was_invalid", missing_invalid);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(512);
    memset(print_buf, 0, 512);
    cJSON_PrintPreallocated(root, print_buf, 512, 0);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "flag");
    cJSON_DeleteItemFromObject(root, "added_flag");
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}