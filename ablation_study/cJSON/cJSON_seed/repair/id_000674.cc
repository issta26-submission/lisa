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
//<ID> 674
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(meta, "name", "cJSON-test");
    cJSON_AddNumberToObject(meta, "version", 2.5);

    // step 2: Configure
    cJSON *settings = cJSON_AddObjectToObject(meta, "settings");
    cJSON_AddTrueToObject(settings, "enabled");
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(123.0));
    cJSON_AddItemToObject(root, "list", arr);

    // step 3: Operate and Validate
    cJSON *name_item = cJSON_GetObjectItem(meta, "name");
    cJSON_bool name_is_str = cJSON_IsString(name_item);
    cJSON_AddBoolToObject(root, "name_is_string", name_is_str);
    cJSON *meta_copy = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_copy", meta_copy);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}