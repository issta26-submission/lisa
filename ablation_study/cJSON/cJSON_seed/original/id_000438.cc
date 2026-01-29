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
//<ID> 438
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"alice\",\"items\":[\"one\",\"two\"]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *new_array = cJSON_CreateArray();
    cJSON *ref1 = cJSON_CreateStringReference("ref_one");
    cJSON *ref2 = cJSON_CreateStringReference("ref_two");
    cJSON_AddItemToArray(new_array, ref1);
    cJSON_AddItemToArray(new_array, ref2);
    cJSON_AddItemToObject(root, "added", new_array);

    // step 3: Operate
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_bool items_invalid = cJSON_IsInvalid(items);
    int new_size = cJSON_GetArraySize(new_array);
    cJSON *first_added = cJSON_GetArrayItem(new_array, 0);
    const char *first_text = cJSON_GetStringValue(first_added);
    (void)root_is_obj;
    (void)items_invalid;
    (void)new_size;
    (void)first_text;
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}