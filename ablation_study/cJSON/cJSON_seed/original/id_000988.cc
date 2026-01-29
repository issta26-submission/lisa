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
//<ID> 988
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"existing\": {}}";
    size_t initial_len = sizeof(initial_json) - 1;
    cJSON *root = cJSON_ParseWithLength(initial_json, initial_len);

    // step 2: Configure
    cJSON *existing = cJSON_GetObjectItem(root, "existing");
    cJSON *nested = cJSON_AddObjectToObject(root, "nested");
    cJSON_bool nested_is_obj = cJSON_IsObject(nested);
    cJSON *temp_item = cJSON_AddStringToObject(nested, "temp", "to_be_removed");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", false_item);
    cJSON_bool flag_was_false = cJSON_IsFalse(false_item);
    cJSON *msg_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(nested, "message", msg_item);
    char *oldptr = cJSON_SetValuestring(msg_item, "updated");

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(256);
    memset(print_buf, 0, 256);
    cJSON_PrintPreallocated(root, print_buf, 256, 0);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(nested, "temp");
    cJSON_DeleteItemFromObjectCaseSensitive(root, "nested");
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}