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
//<ID> 893
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *metrics_key = "metrics";
    const char *count_key = "count";
    cJSON *root = (cJSON *)0;
    cJSON *metrics_arr = (cJSON *)0;
    cJSON *arr_num = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    cJSON *retrieved_metrics = (cJSON *)0;
    char *printed = (char *)0;
    double count_val = 0.0;
    cJSON_bool metrics_is_array = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize / Setup objects
    root = cJSON_CreateObject();
    metrics_arr = cJSON_CreateArray();
    arr_num = cJSON_CreateNumber(123.5);

    // step 3: Configure - build structure and add items
    cJSON_AddItemToArray(metrics_arr, arr_num);
    cJSON_AddItemToObject(root, metrics_key, metrics_arr);
    cJSON_AddItemToObject(root, count_key, cJSON_CreateNumber(42.0));

    // step 4: Operate - retrieve items and examine values
    count_item = cJSON_GetObjectItem(root, count_key);
    count_val = cJSON_GetNumberValue(count_item);
    retrieved_metrics = cJSON_GetObjectItem(root, metrics_key);
    metrics_is_array = cJSON_IsArray(retrieved_metrics);
    printed = cJSON_Print(root);

    // step 5: Validate - produce a simple aggregate score
    validation_score = (int)(root != (cJSON *)0) + (int)(count_item != (cJSON *)0) + (int)metrics_is_array + (int)(retrieved_metrics != (cJSON *)0) + (int)(printed != (char *)0) + (int)count_val;

    // step 6: Cleanup
    if (printed != (char *)0) { cJSON_free(printed); }
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}