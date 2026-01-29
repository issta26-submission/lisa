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
//<ID> 1251
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
    cJSON *s1 = cJSON_CreateString("first");
    cJSON *s2 = cJSON_CreateString("second");
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON_AddItemToObject(root, "items", items);
    cJSON *ref = cJSON_CreateStringReference("ref_string");
    cJSON_AddItemToObject(root, "ref", ref);
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddStringToObject(root, "label", "my_label");

    // step 2: Configure
    cJSON *first_element = cJSON_GetArrayItem(items, 0);
    const char *first_text = cJSON_GetStringValue(first_element);
    cJSON *alias_ref = cJSON_CreateStringReference(first_text);
    cJSON_AddItemToObject(root, "alias", alias_ref);
    cJSON *detached_ref = cJSON_DetachItemFromObjectCaseSensitive(root, "ref");
    cJSON_AddItemToArray(items, detached_ref);

    // step 3: Operate & Validate
    cJSON *third_element = cJSON_GetArrayItem(items, 2);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}