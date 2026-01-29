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
//<ID> 2039
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
    cJSON *str1 = NULL;
    cJSON *str2 = NULL;
    cJSON *got_numbers = NULL;
    cJSON *got_second = NULL;
    char *set_result = NULL;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_num2 = 0;
    cJSON_bool added_arr_to_obj = 0;
    cJSON_bool added_str1 = 0;
    cJSON_bool added_str2 = 0;
    int arr_size = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.0);
    str1 = cJSON_CreateString("alpha");
    str2 = cJSON_CreateString("beta");

    // step 3: Configure
    added_num1 = cJSON_AddItemToArray(arr, num1);
    added_num2 = cJSON_AddItemToArray(arr, num2);
    added_arr_to_obj = cJSON_AddItemToObject(root, "numbers", arr);
    added_str1 = cJSON_AddItemToObject(root, "first", str1);
    added_str2 = cJSON_AddItemToObject(root, "second", str2);

    // step 4: Operate / Inspect
    got_second = cJSON_GetObjectItem(root, "second");
    set_result = cJSON_SetValuestring(got_second, "gamma");
    got_numbers = cJSON_GetObjectItem(root, "numbers");
    arr_size = cJSON_GetArraySize(got_numbers);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(str1 != NULL);
    validation ^= (int)(str2 != NULL);
    validation ^= (int)(got_second != NULL);
    validation ^= (int)(set_result != NULL);
    validation ^= (int)(got_numbers != NULL);
    validation ^= (int)(arr_size == 2);
    validation ^= (int)(added_num1 != 0);
    validation ^= (int)(added_num2 != 0);
    validation ^= (int)(added_arr_to_obj != 0);
    validation ^= (int)(added_str1 != 0);
    validation ^= (int)(added_str2 != 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}