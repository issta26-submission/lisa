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
//<ID> 832
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[3] = { 1.1, 2.2, 3.3 };
    cJSON *data_array = cJSON_CreateDoubleArray(nums, 3);
    cJSON *raw_item = cJSON_CreateRaw("\"raw-segment\"");
    cJSON_AddItemToObjectCS(root, "data", data_array);
    cJSON_AddItemToObjectCS(root, "raw", raw_item);
    cJSON *replacement = cJSON_CreateNumber(9.99);

    // step 2: Configure
    cJSON_ReplaceItemInArray(data_array, 1, replacement);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_data = cJSON_GetObjectItem(parsed, "data");
    cJSON *parsed_item = cJSON_GetArrayItem(parsed_data, 1);
    double extracted_value = cJSON_GetNumberValue(parsed_item);
    (void)extracted_value;
    cJSON *parsed_raw = cJSON_GetObjectItem(parsed, "raw");
    const char *extracted_raw = cJSON_GetStringValue(parsed_raw);
    (void)extracted_raw;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}