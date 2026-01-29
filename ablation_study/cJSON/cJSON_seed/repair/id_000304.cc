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
//<ID> 304
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(root, "count", 5.0);
    cJSON_AddNumberToObject(meta, "inner", 3.14);

    // step 2: Configure
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON *inner_item = cJSON_GetObjectItem(meta, "inner");
    double inner_val = cJSON_GetNumberValue(inner_item);
    double sum = count_val + inner_val;
    cJSON_AddNumberToObject(root, "sum", sum);

    // step 3: Operate and Validate
    char *unformatted = cJSON_PrintUnformatted(root);
    char *pretty = cJSON_Print(root);
    cJSON *sum_item = cJSON_GetObjectItem(root, "sum");
    double sum_val2 = cJSON_GetNumberValue(sum_item);
    cJSON_AddNumberToObject(root, "double_sum", sum_val2 * 2.0);

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(pretty);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}