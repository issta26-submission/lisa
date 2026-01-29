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
//<ID> 65
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
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *n2 = cJSON_CreateNumber(58.0);
    cJSON *flag_item = cJSON_CreateTrue();
    cJSON *retrieved_sum = NULL;
    cJSON *retrieved_numbers = NULL;
    cJSON *retrieved_flag = NULL;
    double sum_val = 0.0;
    cJSON_bool ref_result = 0;
    cJSON_bool flag_true = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "flag", flag_item);
    sum_val = cJSON_GetNumberValue(n1) + cJSON_GetNumberValue(n2);
    retrieved_sum = cJSON_AddNumberToObject(root, "sum", sum_val);

    // step 3: Operate / Validate
    retrieved_numbers = cJSON_GetObjectItem(root, "numbers");
    ref_result = cJSON_AddItemReferenceToArray(retrieved_numbers, retrieved_sum);
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    flag_true = cJSON_IsTrue(retrieved_flag);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)ref_result;
    (void)flag_true;
    return 66;
}