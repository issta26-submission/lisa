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
//<ID> 835
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[3] = {1.1, 2.2, 3.3};
    cJSON *darr = cJSON_CreateDoubleArray(nums, 3);
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *replacement = cJSON_CreateNumber(9.9);

    // step 2: Configure
    cJSON_AddItemToObject(root, "numbers", darr);
    cJSON_AddItemToObjectCS(root, "raw_json", raw_item);

    // step 3: Operate & Validate
    cJSON_ReplaceItemInArray(darr, 1, replacement);
    cJSON *second = cJSON_GetArrayItem(darr, 1);
    double second_val = cJSON_GetNumberValue(second);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_raw = cJSON_GetObjectItemCaseSensitive(parsed, "raw_json");
    const char *raw_str = cJSON_GetStringValue(parsed_raw);
    (void)raw_str;
    (void)second_val;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}