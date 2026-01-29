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
//<ID> 1379
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *stats = cJSON_AddObjectToObject(root, "stats");
    double values[3] = {1.5, 2.5, 3.5};
    cJSON *num_array = cJSON_CreateDoubleArray(values, 3);
    cJSON_AddItemToObject(stats, "values", num_array);

    // step 2: Configure
    cJSON_AddFalseToObject(stats, "active");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_bool replaced = cJSON_ReplaceItemInObject(stats, "active", true_item);
    cJSON *root_flag = cJSON_AddTrueToObject(root, "rootFlag");

    // step 3: Operate and Validate
    cJSON *first_val = cJSON_GetArrayItem(num_array, 0);
    double first_num = cJSON_GetNumberValue(first_val);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    char *buf = (char *)scratch;
    buf[0] = out[0];
    buf[1] = (char)('0' + (int)replaced);
    buf[2] = (char)('0' + ((int)first_num % 10));
    buf[3] = (char)('0' + ((int)first_num / 10 % 10));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}