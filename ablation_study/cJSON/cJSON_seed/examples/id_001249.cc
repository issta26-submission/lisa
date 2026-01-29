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
//<ID> 1249
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"alpha\",\"nums\":[10,20,30],\"mult\":3}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *dup = cJSON_Duplicate(parsed, 1);
    cJSON *nums = cJSON_GetObjectItem(parsed, "nums");
    cJSON *mult = cJSON_GetObjectItem(parsed, "mult");
    cJSON *name = cJSON_GetObjectItem(parsed, "name");

    // step 2: Configure
    cJSON_bool nums_is_array = cJSON_IsArray(nums);
    double first = cJSON_GetNumberValue(cJSON_GetArrayItem(nums, 0));
    double second = cJSON_GetNumberValue(cJSON_GetArrayItem(nums, 1));
    double factor = cJSON_GetNumberValue(mult);
    double computed = (first + second) * factor;
    cJSON_AddNumberToObject(dup, "computed", computed);
    char *name_str = cJSON_GetStringValue(name);
    cJSON_AddStringToObject(dup, "original_name", name_str);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "duplicate", dup);
    cJSON_AddNumberToObject(root, "nums_is_array", (double)nums_is_array);

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}