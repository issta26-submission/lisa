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
//<ID> 439
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"demo\",\"items\":[\"one\",\"two\"]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);
    const cJSON *items = cJSON_GetObjectItem(root, "items");

    // step 2: Configure
    cJSON *new_arr = cJSON_CreateArray();
    cJSON *ref_str = cJSON_CreateStringReference("ref_string");
    cJSON_AddItemToArray(new_arr, ref_str);
    cJSON_AddItemToObject(root, "new_items", new_arr);

    // step 3: Operate
    cJSON_bool root_is_object = cJSON_IsObject(root);
    (void)root_is_object;
    cJSON *missing = cJSON_GetObjectItem(root, "does_not_exist");
    cJSON_bool missing_is_invalid = cJSON_IsInvalid(missing);
    (void)missing_is_invalid;
    int count = cJSON_GetArraySize(items);
    (void)count;
    cJSON *first = cJSON_GetArrayItem(items, 0);
    const char *first_text = cJSON_GetStringValue(first);
    (void)first_text;
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}