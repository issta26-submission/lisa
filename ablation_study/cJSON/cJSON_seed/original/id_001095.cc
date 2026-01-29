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
//<ID> 1095
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"old\",\"values\":[1.1,2.2,3.3],\"nullable\":null,\"num\":5}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *new_name = cJSON_CreateString("new_name");
    cJSON_ReplaceItemViaPointer(root, name_item, new_name);

    // step 3: Operate
    cJSON *values = cJSON_GetObjectItem(root, "values");
    cJSON_bool values_is_array = cJSON_IsArray(values);
    cJSON *first_value = cJSON_GetArrayItem(values, 0);
    double first_number = cJSON_GetNumberValue(first_value);
    cJSON *nullable = cJSON_GetObjectItem(root, "nullable");
    cJSON_bool nullable_is_null = cJSON_IsNull(nullable);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    (void)values_is_array;
    (void)first_number;
    (void)nullable_is_null;
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}