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
//<ID> 2035
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
    cJSON *str_item = NULL;
    cJSON *got_label = NULL;
    cJSON *got_arr = NULL;
    char *set_result = NULL;
    int arr_size = 0;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_num2 = 0;
    cJSON_bool added_arr_to_obj = 0;
    cJSON_bool added_str_to_obj = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.0);
    str_item = cJSON_CreateString("original");

    // step 3: Configure
    added_num1 = cJSON_AddItemToArray(arr, num1);
    added_num2 = cJSON_AddItemToArray(arr, num2);
    added_arr_to_obj = cJSON_AddItemToObject(root, "numbers", arr);
    added_str_to_obj = cJSON_AddItemToObject(root, "label", str_item);

    // step 4: Operate / Inspect
    got_label = cJSON_GetObjectItem(root, "label");
    set_result = cJSON_SetValuestring(got_label, "updated");
    got_arr = cJSON_GetObjectItem(root, "numbers");
    arr_size = cJSON_GetArraySize(got_arr);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(got_label != NULL);
    validation ^= (int)(set_result != NULL);
    validation ^= (int)(got_arr != NULL);
    validation ^= (int)(arr_size == 2);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}