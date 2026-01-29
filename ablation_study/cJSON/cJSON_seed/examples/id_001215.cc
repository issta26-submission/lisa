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
//<ID> 1215
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"values\":[10,20,30],\"keep\":42.5}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *parsed_arr = cJSON_GetObjectItem(parsed, "values");
    cJSON *arr_ref = cJSON_CreateArrayReference(parsed_arr);
    cJSON_AddItemToObject(root, "ref_values", arr_ref);
    cJSON *keep_item = cJSON_GetObjectItem(parsed, "keep");
    double keep_val = cJSON_GetNumberValue(keep_item);
    cJSON_AddNumberToObject(root, "keep_copy", keep_val);
    cJSON *temp = cJSON_CreateNumber(99.0);
    cJSON_AddItemToObject(root, "temp", temp);

    // step 3: Operate & Validate
    cJSON_DeleteItemFromObject(root, "temp");
    cJSON *arr_item1 = cJSON_GetArrayItem(parsed_arr, 1);
    double item1_val = cJSON_GetNumberValue(arr_item1);
    cJSON_AddNumberToObject(root, "item1", item1_val);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}