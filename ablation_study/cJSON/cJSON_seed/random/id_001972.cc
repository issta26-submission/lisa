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
//<ID> 1972
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
    cJSON *got_num = NULL;
    int arr_size = 0;
    double val = 0.0;
    cJSON_bool is_arr = 0;
    cJSON_bool added_to_obj = 0;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_num2 = 0;
    cJSON_bool added_false = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(-1.0);
    false_item = cJSON_CreateFalse();

    // step 3: Core operations
    added_to_obj = cJSON_AddItemToObject(root, "values", arr);
    added_num1 = cJSON_AddItemToArray(arr, num1);
    added_num2 = cJSON_AddItemToArray(arr, num2);
    added_false = cJSON_AddItemToObject(root, "ok", false_item);
    arr_size = cJSON_GetArraySize(arr);
    is_arr = cJSON_IsArray(arr);
    got_num = cJSON_GetArrayItem(arr, 0);
    val = cJSON_GetNumberValue(got_num);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(added_to_obj != 0);
    validation ^= (int)(added_num1 != 0);
    validation ^= (int)(added_num2 != 0);
    validation ^= (int)(added_false != 0);
    validation ^= (int)(arr_size == 2);
    validation ^= (int)(is_arr != 0);
    validation ^= (int)(val == 3.14);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}