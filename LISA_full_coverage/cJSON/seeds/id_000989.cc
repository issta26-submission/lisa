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
//<ID> 989
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"root\": {\"child\": {\"name\": \"old\", \"flag\": false}, \"arr\": [false, true]}, \"value\": 42}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *root_obj = cJSON_GetObjectItem(root, "root");
    cJSON *child = cJSON_GetObjectItem(root_obj, "child");
    cJSON *name_item = cJSON_GetObjectItem(child, "name");
    char *old_name = cJSON_GetStringValue(name_item);
    char *set_name_ptr = cJSON_SetValuestring(name_item, "new_name");
    cJSON_AddStringToObject(root, "previous_name", old_name);
    cJSON_AddStringToObject(root, "name_after_set", set_name_ptr);
    cJSON *arr = cJSON_GetObjectItem(root_obj, "arr");
    cJSON *first_elem = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_was_false = cJSON_IsFalse(first_elem);
    cJSON_AddBoolToObject(root, "first_was_false", first_was_false);
    cJSON *new_sub = cJSON_AddObjectToObject(root, "new_sub");
    cJSON_AddStringToObject(new_sub, "note", "created");
    cJSON_bool new_sub_is_object = cJSON_IsObject(new_sub);
    (void)new_sub_is_object;

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(512);
    memset(print_buf, 0, 512);
    cJSON_PrintPreallocated(root, print_buf, 512, 0);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "value");
    cJSON_DeleteItemFromObjectCaseSensitive(child, "flag");
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}