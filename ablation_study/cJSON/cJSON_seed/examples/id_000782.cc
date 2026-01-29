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
//<ID> 782
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[4] = {3.14, 2.71, 0.577, 1.618};
    cJSON *values_array = cJSON_CreateDoubleArray(nums, 4);
    cJSON *answer_item = cJSON_CreateNumber(42.0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", values_array);
    cJSON_AddItemToObject(root, "answer", answer_item);
    cJSON *values_dup = cJSON_Duplicate(values_array, 1);
    cJSON_AddItemToObject(root, "values_copy", values_dup);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    cJSON_AddItemToObject(parsed, "compare_equal", cJSON_CreateBool(equal));
    cJSON *parsed_dup = cJSON_Duplicate(parsed, 1);
    char *parsed_dup_snapshot = cJSON_PrintUnformatted(parsed_dup);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(parsed_dup_snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed_dup);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}