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
//<ID> 986
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"root\":{\"child\":\"old\"},\"flag\":false}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *root_obj = cJSON_GetObjectItem(root, "root");
    cJSON *child_item = cJSON_GetObjectItem(root_obj, "child");
    char *new_value = cJSON_SetValuestring(child_item, "new");
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_was_false = cJSON_IsFalse(flag_item);
    cJSON_AddBoolToObject(root, "flag_was_false", flag_was_false);
    cJSON *newobj = cJSON_AddObjectToObject(root, "newobj");
    cJSON_bool newobj_is_obj = cJSON_IsObject(newobj);
    cJSON_AddNumberToObject(newobj, "is_obj", (double)newobj_is_obj);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(512);
    memset(print_buf, 0, 512);
    cJSON_PrintPreallocated(root, print_buf, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(newobj, "is_obj");
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}