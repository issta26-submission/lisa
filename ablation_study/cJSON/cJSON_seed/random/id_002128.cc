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
//<ID> 2128
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[32];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *num_created = NULL;
    cJSON *true_item = NULL;
    cJSON *got_flag = NULL;
    cJSON *got_array = NULL;
    cJSON *first_item = NULL;
    double *numbers = NULL;
    double first_val = 0.0;
    int arr_size = 0;
    cJSON_bool is_true = 0;
    int validation = 0;

    // step 2: Setup
    numbers = (double *)cJSON_malloc(sizeof(double) * 2);
    numbers[0] = 1.5;
    numbers[1] = 2.5;
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);
    num1 = cJSON_CreateNumber(numbers[0]);
    num2 = cJSON_CreateNumber(numbers[1]);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Operate
    got_flag = cJSON_GetObjectItemCaseSensitive(root, "flag");
    is_true = cJSON_IsTrue(got_flag);
    got_array = cJSON_GetObjectItemCaseSensitive(root, "values");
    first_item = cJSON_GetArrayItem(got_array, 0);
    first_val = cJSON_GetNumberValue(first_item);
    num_created = cJSON_CreateNumber(first_val * 2.0);
    cJSON_AddItemToArray(got_array, num_created);
    arr_size = cJSON_GetArraySize(got_array);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(arr_size == 3);
    validation ^= (int)(first_val == numbers[0]);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(numbers);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}