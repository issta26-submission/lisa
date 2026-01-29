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
//<ID> 1971
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *false_item = NULL;
    cJSON *first_item = NULL;
    cJSON_bool added_obj = 0;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_num2 = 0;
    cJSON_bool added_flag = 0;
    cJSON_bool is_array = 0;
    double first_val = 0.0;
    int arr_size = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(2.0);
    false_item = cJSON_CreateFalse();
    added_obj = cJSON_AddItemToObject(root, "values", arr);
    added_num1 = cJSON_AddItemToArray(arr, num1);
    added_num2 = cJSON_AddItemToArray(arr, num2);
    added_flag = cJSON_AddItemToObject(root, "enabled", false_item);

    // step 3: Core operations
    arr_size = cJSON_GetArraySize(arr);
    is_array = cJSON_IsArray(arr);
    first_item = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetNumberValue(first_item);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(added_obj != 0);
    validation ^= (int)(added_num1 != 0);
    validation ^= (int)(added_num2 != 0);
    validation ^= (int)(added_flag != 0);
    validation ^= (int)(arr_size == 2);
    validation ^= (int)(is_array != 0);
    validation ^= (int)(first_val == 3.14);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}