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
//<ID> 677
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *str0 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, str0);
    cJSON *bool1 = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, bool1);

    // step 2: Configure
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddStringToObject(config, "key", "original");

    // step 3: Operate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_is_string = cJSON_IsString(first);
    cJSON_AddBoolToObject(config, "first_is_string", first_is_string);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    cJSON_bool second_is_bool = cJSON_IsBool(second);
    cJSON_AddBoolToObject(config, "second_is_bool", second_is_bool);
    int arr_size = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(config, "size", (double)arr_size);
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_bool replaced_ok = cJSON_ReplaceItemInObject(config, "key", replacement);
    cJSON_bool replacement_is_string = cJSON_IsString(replacement);
    cJSON_AddBoolToObject(root, "replacement_is_string", replacement_is_string);
    (void)replaced_ok;

    // step 4: Validate & Cleanup
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}