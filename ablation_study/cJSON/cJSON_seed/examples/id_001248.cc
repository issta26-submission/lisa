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
//<ID> 1248
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"example\",\"values\":[4,5,6],\"factor\":3}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *name = cJSON_GetObjectItem(parsed, "name");
    cJSON *values = cJSON_GetObjectItem(parsed, "values");
    cJSON *factor = cJSON_GetObjectItem(parsed, "factor");
    cJSON *values_dup = cJSON_Duplicate(values, 1);
    cJSON_bool array_flag = cJSON_IsArray(values_dup);
    cJSON_AddStringToObject(root, "source_name", cJSON_GetStringValue(name));
    cJSON_AddNumberToObject(root, "is_array", (double)array_flag);

    // step 3: Operate
    cJSON *first = cJSON_GetArrayItem(values_dup, 0);
    double first_val = cJSON_GetNumberValue(first);
    double factor_val = cJSON_GetNumberValue(factor);
    cJSON_AddNumberToObject(root, "factor", factor_val);
    cJSON_AddNumberToObject(root, "computed", first_val * factor_val);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(values_dup);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}