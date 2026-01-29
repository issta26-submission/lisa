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
//<ID> 1411
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddNumberToObject(root, "threshold", 2.5);
    cJSON_AddBoolToObject(root, "active", 1);
    cJSON *itemA = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", itemA);

    // step 2: Configure
    cJSON *dup_answer = cJSON_Duplicate(itemA, 1);
    cJSON_AddItemToObject(root, "answer_copy", dup_answer);
    cJSON *get_answer = cJSON_GetObjectItem(root, "answer");
    cJSON_AddItemReferenceToArray(values, get_answer);
    cJSON *get_threshold = cJSON_GetObjectItem(root, "threshold");
    cJSON_AddItemReferenceToArray(values, get_threshold);

    // step 3: Operate & Validate
    int arr_size = cJSON_GetArraySize(values);
    cJSON *retrieved_dup = cJSON_GetObjectItem(root, "answer_copy");
    double current = cJSON_GetNumberValue(retrieved_dup);
    cJSON_SetNumberHelper(retrieved_dup, current + 1.0);
    double new_val = cJSON_GetNumberValue(retrieved_dup);
    cJSON_AddNumberToObject(root, "sum_summary", (double)arr_size + new_val);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}