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
//<ID> 1212
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"values\":[10,20,30],\"remove_me\":999}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *values = cJSON_GetObjectItem(parsed, "values");
    cJSON *item_index_1 = cJSON_GetArrayItem(values, 1);
    double extracted = cJSON_GetNumberValue(item_index_1);
    cJSON *ref_values = cJSON_CreateArrayReference(values);
    cJSON_AddItemToObject(root, "values_ref", ref_values);
    cJSON_AddNumberToObject(root, "extracted", extracted);
    cJSON_DeleteItemFromObject(parsed, "remove_me");

    // step 3: Operate & Validate
    cJSON *ref_from_root = cJSON_GetObjectItem(root, "values_ref");
    cJSON *ref_item0 = cJSON_GetArrayItem(ref_from_root, 0);
    double first_val = cJSON_GetNumberValue(ref_item0);
    cJSON_AddNumberToObject(root, "sum_extracted_and_first", extracted + first_val);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}