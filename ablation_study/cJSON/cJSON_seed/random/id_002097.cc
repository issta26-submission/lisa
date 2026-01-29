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
//<ID> 2097
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
    cJSON *num_item = NULL;
    cJSON *raw_item = NULL;
    cJSON *arr_num = NULL;
    cJSON *got_num_obj = NULL;
    cJSON *got_raw_obj = NULL;
    cJSON *arr_item0 = NULL;
    double extracted_value = 0.0;
    cJSON_bool raw_check = 0;
    cJSON_bool num_check = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(3.1415);
    raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    arr_num = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToObject(root, "pi", num_item);
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON_AddItemToArray(arr, arr_num);
    cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate
    got_num_obj = cJSON_GetObjectItem(root, "pi");
    got_raw_obj = cJSON_GetObjectItem(root, "raw");
    arr_item0 = cJSON_GetArrayItem(arr, 0);
    extracted_value = cJSON_GetNumberValue(got_num_obj);
    raw_check = cJSON_IsRaw(got_raw_obj);
    num_check = cJSON_IsNumber(arr_item0);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(raw_item != NULL);
    validation ^= (int)(arr_num != NULL);
    validation ^= (int)(got_num_obj != NULL);
    validation ^= (int)(got_raw_obj != NULL);
    validation ^= (int)(arr_item0 != NULL);
    validation ^= (int)(extracted_value == 3.1415);
    validation ^= (int)(raw_check != 0);
    validation ^= (int)(num_check != 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}