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
//<ID> 982
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"root\":{\"child\":false,\"text\":\"hello\"}}";
    cJSON *root = cJSON_Parse(initial_json);
    cJSON *root_obj = cJSON_GetObjectItem(root, "root");

    // step 2: Configure
    cJSON *child = cJSON_GetObjectItemCaseSensitive(root_obj, "child");
    cJSON_bool child_was_false = cJSON_IsFalse(child);
    cJSON *added = cJSON_AddObjectToObject(root_obj, "added");
    cJSON_bool added_is_object = cJSON_IsObject(added);
    cJSON *text = cJSON_GetObjectItem(root_obj, "text");
    char *set_res = cJSON_SetValuestring(text, "world");
    char *current_text = cJSON_GetStringValue(text);
    cJSON_AddStringToObject(added, "text_copy", current_text);
    cJSON_AddBoolToObject(root_obj, "child_was_false", child_was_false);
    cJSON_AddBoolToObject(root, "added_is_object", added_is_object);

    // step 3: Operate & Validate
    cJSON *note = cJSON_AddStringToObject(added, "note", "added");
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(added, "flag", flag);
    cJSON_DeleteItemFromObjectCaseSensitive(root_obj, "child");

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_PrintPreallocated(root, buffer, 256, 0);
    cJSON_free(out);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}