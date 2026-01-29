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
//<ID> 2001
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
    cJSON *label_item = NULL;
    cJSON *arr_from_root = NULL;
    cJSON *bool_item = NULL;
    char *got_str = NULL;
    int arr_size = 0;
    int validation = 0;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_num2 = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_label = 0;

    // step 2: Setup (create root, array, numbers, and string)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(2.71);
    str_item = cJSON_CreateString("example");

    // step 3: Configure (add numbers to array and attach items to root; add boolean)
    added_num1 = cJSON_AddItemToArray(arr, num1);
    added_num2 = cJSON_AddItemToArray(arr, num2);
    added_arr = cJSON_AddItemToObject(root, "numbers", arr);
    added_label = cJSON_AddItemToObject(root, "label", str_item);
    bool_item = cJSON_AddBoolToObject(root, "valid", 1);

    // step 4: Operate (retrieve string value and get array size via object)
    label_item = cJSON_GetObjectItem(root, "label");
    got_str = cJSON_GetStringValue(label_item);
    arr_from_root = cJSON_GetObjectItem(root, "numbers");
    arr_size = cJSON_GetArraySize(arr_from_root);

    // step 5: Validate (aggregate simple checks into buffer)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(got_str != NULL);
    validation ^= (int)(arr_size == 2);
    validation ^= (int)(added_num1 != 0);
    validation ^= (int)(added_num2 != 0);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(added_label != 0);
    validation ^= (int)(bool_item != NULL);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}