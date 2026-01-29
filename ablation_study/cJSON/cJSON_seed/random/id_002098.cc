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
//<ID> 2098
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[8];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num_pi = NULL;
    cJSON *arr_num = NULL;
    cJSON *arr_raw = NULL;
    cJSON *got_arr = NULL;
    cJSON *got_pi = NULL;
    cJSON *got_arr0 = NULL;
    cJSON *got_arr1 = NULL;
    double pi_val = 0.0;
    double arr0_val = 0.0;
    cJSON_bool pi_is_number = 0;
    cJSON_bool arr0_is_number = 0;
    cJSON_bool arr1_is_raw = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_pi = cJSON_CreateNumber(3.1415);
    arr_num = cJSON_CreateNumber(42.0);
    arr_raw = cJSON_CreateRaw("unparsed-raw");
    cJSON_AddItemToArray(arr, arr_num);
    cJSON_AddItemToArray(arr, arr_raw);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToObject(root, "pi", num_pi);

    // step 3: Operate
    got_arr = cJSON_GetObjectItem(root, "arr");
    got_pi = cJSON_GetObjectItem(root, "pi");
    got_arr0 = cJSON_GetArrayItem(got_arr, 0);
    got_arr1 = cJSON_GetArrayItem(got_arr, 1);
    pi_val = cJSON_GetNumberValue(got_pi);
    arr0_val = cJSON_GetNumberValue(got_arr0);
    pi_is_number = cJSON_IsNumber(got_pi);
    arr0_is_number = cJSON_IsNumber(got_arr0);
    arr1_is_raw = cJSON_IsRaw(got_arr1);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num_pi != NULL);
    validation ^= (int)(arr_num != NULL);
    validation ^= (int)(arr_raw != NULL);
    validation ^= (int)(got_arr != NULL);
    validation ^= (int)(got_pi != NULL);
    validation ^= (int)(got_arr0 != NULL);
    validation ^= (int)(got_arr1 != NULL);
    validation ^= (int)(pi_is_number != 0);
    validation ^= (int)(arr0_is_number != 0);
    validation ^= (int)(arr1_is_raw != 0);
    validation ^= (int)(pi_val == 3.1415);
    validation ^= (int)(arr0_val == 42.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}