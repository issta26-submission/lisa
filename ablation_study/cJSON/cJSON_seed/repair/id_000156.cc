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
//<ID> 156
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", data);

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(4.0);
    cJSON_AddItemToArray(data, num1);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToArray(data, flag);
    cJSON *num2 = cJSON_CreateNumber(6.0);
    cJSON_AddItemToArray(data, num2);
    cJSON *threshold = cJSON_CreateNumber(5.0);
    cJSON_AddItemToObject(root, "threshold", threshold);

    // step 3: Operate and Validate
    cJSON *it0 = cJSON_GetArrayItem(data, 0);
    cJSON *it2 = cJSON_GetArrayItem(data, 2);
    double v0 = cJSON_GetNumberValue(it0);
    double v2 = cJSON_GetNumberValue(it2);
    double sum = v0 + v2;
    double avg = sum / 2.0;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON *avg_item = cJSON_CreateNumber(avg);
    cJSON_AddItemToObject(root, "average", avg_item);
    cJSON *thr_obj = cJSON_GetObjectItem(root, "threshold");
    double thr_val = cJSON_GetNumberValue(thr_obj);
    double diff = sum - thr_val;
    cJSON *diff_item = cJSON_CreateNumber(diff);
    cJSON_AddItemToObject(root, "diff", diff_item);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}