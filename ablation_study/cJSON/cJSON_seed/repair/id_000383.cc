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
//<ID> 383
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version_str = cJSON_Version();
    const char *json_text = "{\"numbers\":[1,2,3],\"info\":{\"name\":\"alpha\",\"active\":false}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *n0 = cJSON_GetArrayItem(numbers, 0);
    cJSON *n1 = cJSON_GetArrayItem(numbers, 1);
    double partial_sum_val = cJSON_GetNumberValue(n0) + cJSON_GetNumberValue(n1);
    cJSON *partial_sum = cJSON_CreateNumber(partial_sum_val);
    cJSON_AddItemToObject(root, "partial_sum", partial_sum);
    cJSON *meta_array = cJSON_CreateArray();
    cJSON_AddItemToArray(meta_array, cJSON_CreateBool(cJSON_HasObjectItem(root, "info")));
    cJSON_AddItemToArray(meta_array, cJSON_CreateString(version_str));
    cJSON_AddItemToObject(root, "meta", meta_array);

    // step 3: Operate and Validate
    cJSON *info = cJSON_GetObjectItemCaseSensitive(root, "info");
    cJSON *name_item = cJSON_GetObjectItem(info, "name");
    const char *extracted_name = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "extracted_name", extracted_name);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}