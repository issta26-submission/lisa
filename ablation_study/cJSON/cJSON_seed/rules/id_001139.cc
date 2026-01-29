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
//<ID> 1139
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *items = NULL;
    cJSON *first = NULL;
    cJSON *second = NULL;
    cJSON *dup_first = NULL;
    cJSON *target_array = NULL;
    cJSON *replacement_obj = NULL;
    cJSON *meta = NULL;
    const char *first_name = NULL;
    const char *second_name = NULL;
    const char json[] = "{\"items\":[{\"name\":\"one\"},{\"name\":\"two\"}],\"meta\":{\"key\":\"value\"}}";

    // step 2: Setup / Configure
    parsed = cJSON_Parse(json);
    items = cJSON_GetObjectItem(parsed, "items");
    first = cJSON_GetArrayItem(items, 0);
    second = cJSON_GetArrayItem(items, 1);
    first_name = cJSON_GetStringValue(cJSON_GetObjectItem(first, "name"));
    second_name = cJSON_GetStringValue(cJSON_GetObjectItem(second, "name"));

    // step 3: Operate / Validate
    dup_first = cJSON_Duplicate(first, 1);
    target_array = cJSON_CreateArray();
    cJSON_AddItemReferenceToArray(target_array, dup_first);
    replacement_obj = cJSON_CreateObject();
    cJSON_AddItemToObject(replacement_obj, "name", cJSON_CreateString("two_replaced"));
    cJSON_ReplaceItemInArray(items, 1, replacement_obj);
    meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON_ReplaceItemInObjectCaseSensitive(meta, "key", cJSON_CreateString("new_value"));
    (void)first_name;
    (void)second_name;

    // step 4: Cleanup
    cJSON_Delete(target_array);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}