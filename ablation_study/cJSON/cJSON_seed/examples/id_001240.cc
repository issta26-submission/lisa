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
//<ID> 1240
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"alpha\",\"values\":[4,5,6],\"meta\":2}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *values = cJSON_GetObjectItem(parsed, "values");
    cJSON_bool is_array = cJSON_IsArray(values);
    cJSON_AddNumberToObject(root, "values_is_array", (double)is_array);
    cJSON *dup_parsed = cJSON_Duplicate(parsed, 1);
    cJSON *values_dup = cJSON_GetObjectItem(dup_parsed, "values");
    cJSON *values_copy = cJSON_Duplicate(values_dup, 1);
    cJSON_AddItemToObject(root, "values_copy", values_copy);

    // step 3: Operate & Validate
    cJSON *v0 = cJSON_GetArrayItem(values, 0);
    cJSON *v1 = cJSON_GetArrayItem(values, 1);
    cJSON *v2 = cJSON_GetArrayItem(values, 2);
    double sum = cJSON_GetNumberValue(v0) + cJSON_GetNumberValue(v1) + cJSON_GetNumberValue(v2);
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON *name_item = cJSON_GetObjectItem(parsed, "name");
    char *name_val = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "name_copy", name_val ? name_val : "");
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    cJSON_Delete(dup_parsed);

    return 66;
    // API sequence test completed successfully
}