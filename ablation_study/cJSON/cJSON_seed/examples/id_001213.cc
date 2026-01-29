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
//<ID> 1213
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"values\":[10,20,30],\"delete_me\":999,\"label\":\"sample\"}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *values = cJSON_GetObjectItem(parsed, "values");
    cJSON *values_ref = cJSON_CreateArrayReference(values);
    cJSON_AddItemToObject(root, "values_ref", values_ref);
    cJSON *item1 = cJSON_GetArrayItem(values, 1);
    double item1_val = cJSON_GetNumberValue(item1);
    cJSON_AddNumberToObject(root, "item1_value", item1_val);
    cJSON_DeleteItemFromObject(parsed, "delete_me");

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}