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
//<ID> 2033
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
    cJSON *str1 = NULL;
    cJSON *str2 = NULL;
    cJSON *num = NULL;
    cJSON *greeting = NULL;
    cJSON *arr_from_obj = NULL;
    char *printed = NULL;
    char *set_ret = NULL;
    int arr_size = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added3 = 0;
    cJSON_bool added_arr_to_obj = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("hello");
    str2 = cJSON_CreateString("world");
    num = cJSON_CreateNumber(42.0);

    // step 3: Configure
    added1 = cJSON_AddItemToArray(arr, str1);
    added2 = cJSON_AddItemToArray(arr, str2);
    added3 = cJSON_AddItemToArray(arr, num);
    added_arr_to_obj = cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToObject(root, "greeting", cJSON_CreateString("initial"));
    greeting = cJSON_GetObjectItem(root, "greeting");
    set_ret = cJSON_SetValuestring(greeting, "hi");

    // step 4: Operate / Inspect
    arr_from_obj = cJSON_GetObjectItem(root, "arr");
    arr_size = cJSON_GetArraySize(arr_from_obj);
    greeting = cJSON_GetObjectItem(root, "greeting");

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(str1 != NULL);
    validation ^= (int)(str2 != NULL);
    validation ^= (int)(num != NULL);
    validation ^= (int)(added1 != 0);
    validation ^= (int)(added2 != 0);
    validation ^= (int)(added3 != 0);
    validation ^= (int)(added_arr_to_obj != 0);
    validation ^= (int)(arr_size == 3);
    validation ^= (int)(greeting != NULL);
    validation ^= (int)(set_ret != NULL);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}