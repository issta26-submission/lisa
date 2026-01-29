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
//<ID> 983
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"config\":{\"name\":\"old\"},\"flag\":false,\"value\":\"initial\"}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *config = cJSON_GetObjectItem(root, "config");
    cJSON *flag = cJSON_GetObjectItem(root, "flag");
    cJSON *value = cJSON_GetObjectItem(root, "value");
    cJSON *added = cJSON_AddObjectToObject(root, "added");
    cJSON_AddStringToObject(added, "inner", "hello");
    char *new_value_str = cJSON_SetValuestring(value, "updated");

    // step 3: Operate
    cJSON_bool flag_was_false = cJSON_IsFalse(flag);
    cJSON_bool added_is_object = cJSON_IsObject(added);
    char *out = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON_PrintPreallocated(root, buffer, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "flag");
    cJSON_free(out);
    cJSON_free(buffer);
    cJSON_free(new_value_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}