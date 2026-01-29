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
//<ID> 1211
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"obj\":{\"nums\":[1.5,2.5,3.5]},\"flag\":true}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *obj = cJSON_GetObjectItem(parsed, "obj");
    cJSON *nums = cJSON_GetObjectItem(obj, "nums");
    cJSON *first = cJSON_GetArrayItem(nums, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *ref_array = cJSON_CreateArrayReference(first);
    cJSON_AddItemToObject(root, "referenced_first", ref_array);
    cJSON_AddNumberToObject(root, "first_value", first_val);

    // step 3: Operate & Validate
    cJSON_DeleteItemFromObject(parsed, "flag");
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}