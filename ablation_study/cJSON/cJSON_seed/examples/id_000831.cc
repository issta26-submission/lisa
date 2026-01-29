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
//<ID> 831
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[3] = { 1.1, 2.2, 3.3 };
    cJSON *double_arr = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *raw_item = cJSON_CreateRaw("RAW_CONTENT");

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "values", double_arr);
    cJSON_AddItemToObjectCS(root, "metadata", raw_item);
    cJSON *values_ref = cJSON_GetObjectItem(root, "values");

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateNumber(4.4);
    cJSON_ReplaceItemInArray(values_ref, 1, replacement);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    (void)equal;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}