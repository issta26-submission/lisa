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
//<ID> 67
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(13.0);
    cJSON *n2 = cJSON_CreateNumber(29.0);
    cJSON *flag = cJSON_CreateTrue();
    cJSON *sum_item = NULL;
    cJSON *retrieved_sum = NULL;
    cJSON *retrieved_flag = NULL;
    double n1_val = 0.0;
    double n2_val = 0.0;
    double sum_val = 0.0;
    cJSON_bool flag_truth = 0;
    cJSON_bool added_ref = 0;

    // step 2: Setup / Configure structure
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, n1);
    added_ref = cJSON_AddItemReferenceToArray(arr, n2);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate / Validate
    n1_val = cJSON_GetNumberValue(n1);
    n2_val = cJSON_GetNumberValue(n2);
    sum_item = cJSON_AddNumberToObject(root, "sum", n1_val + n2_val + (double)added_ref);
    retrieved_sum = cJSON_GetObjectItem(root, "sum");
    sum_val = cJSON_GetNumberValue(retrieved_sum);
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    flag_truth = cJSON_IsTrue(retrieved_flag);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)sum_val;
    (void)flag_truth;
    return 66;
}