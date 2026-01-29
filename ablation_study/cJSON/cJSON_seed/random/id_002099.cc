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
//<ID> 2099
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[16];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *raw_item = NULL;
    cJSON *arr = NULL;
    cJSON *got_num = NULL;
    cJSON *got_raw = NULL;
    cJSON *got_arr = NULL;
    cJSON *arr_num0 = NULL;
    double num_val = 0.0;
    double arr_val = 0.0;
    cJSON_bool is_raw = 0;
    cJSON_bool is_number = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(root, "pi", num_item);
    raw_item = cJSON_CreateRaw("{\"x\":1}");
    cJSON_AddItemToObject(root, "raw", raw_item);
    arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(20.0));
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate
    got_num = cJSON_GetObjectItem(root, "pi");
    num_val = cJSON_GetNumberValue(got_num);
    got_raw = cJSON_GetObjectItem(root, "raw");
    is_raw = cJSON_IsRaw(got_raw);
    got_arr = cJSON_GetObjectItem(root, "values");
    arr_num0 = cJSON_GetArrayItem(got_arr, 0);
    is_number = cJSON_IsNumber(arr_num0);
    arr_val = cJSON_GetNumberValue(arr_num0);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(raw_item != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(got_num != NULL);
    validation ^= (int)(got_raw != NULL);
    validation ^= (int)(got_arr != NULL);
    validation ^= (int)(arr_num0 != NULL);
    validation ^= (int)(num_val == 3.14);
    validation ^= (int)(arr_val == 10.0);
    validation ^= (int)(is_raw != 0);
    validation ^= (int)(is_number != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}