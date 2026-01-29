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
//<ID> 192
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
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);

    // step 2: Configure
    cJSON_AddStringToObject(root, "description", "sample array");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON *first_copy = cJSON_CreateString(cJSON_GetStringValue(first));
    cJSON_AddItemToObject(meta, "copied_first", first_copy);

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(items, 1);
    cJSON_bool eq = cJSON_Compare(first, first_copy, 1);
    cJSON_AddBoolToObject(meta, "first_equals_copy", eq);
    cJSON_AddStringToObject(meta, "second_value", cJSON_GetStringValue(second));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}