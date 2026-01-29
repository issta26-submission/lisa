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
//<ID> 2009
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
    cJSON *bool_item = NULL;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_num2 = 0;
    cJSON_bool added_arr = 0;
    char *got_str = NULL;
    int arr_size = 0;
    int validation = 0;

    // step 2: Setup (Initialize root and items)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(7.0);
    num2 = cJSON_CreateNumber(14.0);
    str1 = cJSON_CreateString("example");

    // step 3: Configure (populate array and object, add boolean)
    added_num1 = cJSON_AddItemToArray(arr, num1);
    added_num2 = cJSON_AddItemToArray(arr, num2);
    added_arr = cJSON_AddItemToObject(root, "values", arr);
    bool_item = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddItemToObject(root, "label", str1);

    // step 4: Operate (inspect array and retrieve string)
    arr_size = cJSON_GetArraySize(arr);
    got_str = cJSON_GetStringValue(str1);

    // step 5: Validate (aggregate simple checks into buffer)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(str1 != NULL);
    validation ^= (int)(added_num1 != 0);
    validation ^= (int)(added_num2 != 0);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(got_str != NULL);
    validation ^= (int)(arr_size == 2);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}