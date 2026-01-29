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
//<ID> 2095
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
    cJSON *obj_num = NULL;
    cJSON *arr_num = NULL;
    cJSON *arr_raw = NULL;
    cJSON *got_obj_item = NULL;
    cJSON *got_arr_item0 = NULL;
    cJSON *got_arr_item1 = NULL;
    double val_obj = 0.0;
    double val_arr0 = 0.0;
    cJSON_bool is_num = 0;
    cJSON_bool is_raw = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    obj_num = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(root, "pi", obj_num);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    arr_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, arr_num);
    arr_raw = cJSON_CreateRaw("raw_payload");
    cJSON_AddItemToArray(arr, arr_raw);

    // step 3: Operate
    got_obj_item = cJSON_GetObjectItem(root, "pi");
    val_obj = cJSON_GetNumberValue(got_obj_item);
    cJSON *got_list = cJSON_GetObjectItem(root, "list");
    got_arr_item0 = cJSON_GetArrayItem(got_list, 0);
    val_arr0 = cJSON_GetNumberValue(got_arr_item0);
    got_arr_item1 = cJSON_GetArrayItem(got_list, 1);
    is_num = cJSON_IsNumber(got_arr_item0);
    is_raw = cJSON_IsRaw(got_arr_item1);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(obj_num != NULL);
    validation ^= (int)(arr_num != NULL);
    validation ^= (int)(arr_raw != NULL);
    validation ^= (int)(got_obj_item != NULL);
    validation ^= (int)(got_arr_item0 != NULL);
    validation ^= (int)(got_arr_item1 != NULL);
    validation ^= (int)(val_obj == 3.14);
    validation ^= (int)(val_arr0 == 42.0);
    validation ^= (int)(is_num != 0);
    validation ^= (int)(is_raw != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}