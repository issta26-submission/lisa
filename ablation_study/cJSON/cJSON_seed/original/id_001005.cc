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
//<ID> 1005
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"root\":{\"arr\":[10,2.5,false],\"meta\":{\"ok\":true}},\"value\":123}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(initial_json, &parse_end, 1);

    // step 2: Configure
    cJSON *root_obj = cJSON_GetObjectItem(root, "root");
    cJSON *arr = cJSON_GetObjectItem(root_obj, "arr");
    cJSON *first_elem = cJSON_GetArrayItem(arr, 0);
    cJSON *second_elem = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_number = cJSON_IsNumber(second_elem);
    cJSON_AddBoolToObject(root, "second_is_number", second_is_number);
    cJSON *explicit_true = cJSON_AddTrueToObject(root, "explicit_true");
    cJSON *temp_null = cJSON_CreateNull();
    cJSON_bool null_is_invalid = cJSON_IsInvalid(temp_null);
    cJSON_AddBoolToObject(root, "null_was_invalid", null_is_invalid);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(512);
    memset(print_buf, 0, 512);
    cJSON_PrintPreallocated(root, print_buf, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "second_is_number");
    cJSON_DeleteItemFromObject(root, "null_was_invalid");
    cJSON_DeleteItemFromObject(root, "explicit_true");
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(temp_null);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}