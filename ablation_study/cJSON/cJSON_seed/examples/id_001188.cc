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
//<ID> 1188
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"items\":[10,20],\"flag\":false}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *generated_arr = cJSON_CreateArray();
    cJSON *g1 = cJSON_CreateNumber(1.0);
    cJSON *g2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(generated_arr, g1);
    cJSON_AddItemToArray(generated_arr, g2);
    cJSON_AddItemToObject(root, "generated", generated_arr);
    cJSON_AddNullToObject(root, "optional");
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate & Validate
    cJSON *parsed_items = cJSON_GetObjectItem(parsed, "items");
    cJSON *first_item = cJSON_GetArrayItem(parsed_items, 0);
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemViaPointer(parsed_items, first_item, replacement);
    cJSON *updated_item = cJSON_GetArrayItem(parsed_items, 0);
    double updated_val = cJSON_GetNumberValue(updated_item);
    cJSON_AddNumberToObject(root, "replaced_value", updated_val);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}