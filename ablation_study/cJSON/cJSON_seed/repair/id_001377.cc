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
//<ID> 1377
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[3] = {1.5, 2.75, 3.125};
    cJSON *arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddTrueToObject(root, "flag");
    cJSON_AddNumberToObject(root, "score", 42.0);
    cJSON *true_node = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "flag", true_node);

    // step 3: Operate and Validate
    cJSON *values_obj = cJSON_GetObjectItem(root, "values");
    cJSON *first_elem = cJSON_GetArrayItem(values_obj, 0);
    double first_val = cJSON_GetNumberValue(first_elem);
    cJSON *score_item = cJSON_GetObjectItem(root, "score");
    double score_val = cJSON_GetNumberValue(score_item);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    char *buf = (char *)scratch;
    buf[0] = out[0];
    buf[1] = (char)('0' + (int)replaced);
    buf[2] = (char)('0' + ((int)first_val % 10));
    buf[3] = (char)('0' + ((int)score_val % 10));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}