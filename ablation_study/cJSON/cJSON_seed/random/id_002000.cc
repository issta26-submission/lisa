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
//<ID> 2000
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
    cJSON *str_item = NULL;
    cJSON *got_name = NULL;
    char *name_value = NULL;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_num2 = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_name = 0;
    cJSON *flag_item = NULL;
    cJSON_bool added_flag = 0;
    int arr_size = 0;
    int validation = 0;

    // step 2: Setup (Initialize root, array, numbers, and string)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);
    str_item = cJSON_CreateString("example");

    // step 3: Configure (build array and object; add boolean using required API)
    added_num1 = cJSON_AddItemToArray(arr, num1);
    added_num2 = cJSON_AddItemToArray(arr, num2);
    added_arr = cJSON_AddItemToObject(root, "data", arr);
    added_name = cJSON_AddItemToObject(root, "name", str_item);
    flag_item = cJSON_AddBoolToObject(root, "flag", 1);
    added_flag = (flag_item != NULL);

    // step 4: Operate (inspect string value and array size using required APIs)
    got_name = cJSON_GetObjectItem(root, "name");
    name_value = cJSON_GetStringValue(got_name);
    arr_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "data"));

    // step 5: Validate (aggregate simple checks into buffer)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(added_num1 != 0);
    validation ^= (int)(added_num2 != 0);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(added_name != 0);
    validation ^= (int)(added_flag != 0);
    validation ^= (int)(name_value != NULL);
    validation ^= (int)(arr_size == 2);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}