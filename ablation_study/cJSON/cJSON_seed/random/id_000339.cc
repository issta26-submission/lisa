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
//<ID> 339
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *dup = (cJSON *)0;
    char *first_str = (char *)0;
    int arr_size = 0;
    cJSON_bool is_str = (cJSON_bool)0;
    cJSON_bool is_obj = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    cJSON_AddItemToObject(root, "items", arr);
    str1 = cJSON_CreateString("alpha");
    str2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    first_elem = cJSON_GetArrayItem(arr, 0);
    first_str = cJSON_GetStringValue(first_elem);
    is_str = cJSON_IsString(first_elem);
    is_obj = cJSON_IsObject(root);
    dup = cJSON_Duplicate(root, (cJSON_bool)1);

    // step 5: Validate
    cJSON_GetObjectItem(dup, "flag");
    cJSON_GetArrayItem(dup, 1);
    (void)first_str;
    (void)arr_size;
    (void)is_str;
    (void)is_obj;

    // step 6: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}