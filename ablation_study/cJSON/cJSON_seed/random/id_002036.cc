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
//<ID> 2036
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
    cJSON *stritem = NULL;
    cJSON *got_name = NULL;
    cJSON *got_arr = NULL;
    char *setret = NULL;
    int arr_size = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_name = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.0);
    stritem = cJSON_CreateString("old_label");

    // step 3: Configure
    added1 = cJSON_AddItemToArray(arr, num1);
    added2 = cJSON_AddItemToArray(arr, num2);
    added_arr = cJSON_AddItemToObject(root, "numbers", arr);
    added_name = cJSON_AddItemToObject(root, "label", stritem);
    setret = cJSON_SetValuestring(stritem, "new_label");

    // step 4: Operate / Inspect
    arr_size = cJSON_GetArraySize(arr);
    got_name = cJSON_GetObjectItem(root, "label");
    got_arr = cJSON_GetObjectItem(root, "numbers");

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(stritem != NULL);
    validation ^= (int)(added1 != 0);
    validation ^= (int)(added2 != 0);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(added_name != 0);
    validation ^= (int)(arr_size == 2);
    validation ^= (int)(got_name != NULL);
    validation ^= (int)(got_arr != NULL);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}