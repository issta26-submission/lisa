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
//<ID> 1247
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"example\",\"items\":[1,2,3],\"mult\":2}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *items = cJSON_GetObjectItem(parsed, "items");
    cJSON *name = cJSON_GetObjectItem(parsed, "name");
    cJSON *mult = cJSON_GetObjectItem(parsed, "mult");

    // step 2: Configure
    cJSON *dup_items = cJSON_Duplicate(items, 1);
    cJSON_bool is_array = cJSON_IsArray(dup_items);
    cJSON *root = cJSON_CreateObject();
    cJSON *second = cJSON_GetArrayItem(items, 1);
    double second_val = cJSON_GetNumberValue(second);
    double factor = cJSON_GetNumberValue(mult);
    cJSON_AddNumberToObject(root, "computed", second_val * factor);
    cJSON_AddStringToObject(root, "original_name", cJSON_GetStringValue(name));
    cJSON_AddNumberToObject(root, "dup_is_array", (double)is_array);
    cJSON_AddItemToObject(root, "duplicated_items", dup_items);

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}