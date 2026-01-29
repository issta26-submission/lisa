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
//<ID> 552
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_AddObjectToObject(root, "metrics");
    cJSON *num_initial = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(metrics, "initial", num_initial);
    cJSON *num_src = cJSON_CreateNumber(1.5);
    cJSON_AddItemToObject(root, "src", num_src);

    // step 2: Configure
    cJSON *added = cJSON_AddNumberToObject(metrics, "added", 3.5);
    cJSON_SetNumberHelper(added, 7.25);
    cJSON *dup_num = cJSON_Duplicate(num_src, 0);
    cJSON_AddItemToObject(root, "dup_num", dup_num);

    // step 3: Operate and Validate
    double v1 = cJSON_GetNumberValue(cJSON_GetObjectItem(metrics, "initial"));
    double v2 = cJSON_GetNumberValue(cJSON_GetObjectItem(metrics, "added"));
    double v3 = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "src"));
    double v4 = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "dup_num"));
    double sum = v1 + v2 + v3 + v4;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    char *out_dup = cJSON_PrintUnformatted(root_dup);
    cJSON_free(out_dup);
    cJSON_Delete(root_dup);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}