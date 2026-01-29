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
//<ID> 1631
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
    cJSON *meta = (cJSON *)0;
    cJSON *num_ptr = (cJSON *)0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added3 = 0;
    cJSON_bool is_str = 0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("hello");
    str2 = cJSON_CreateString("world");
    meta = cJSON_CreateObject();
    num_ptr = cJSON_AddNumberToObject(meta, "count", 42.0);
    num_ptr = cJSON_AddNumberToObject(root, "pi", 3.1415);

    // step 3: Configure
    added1 = cJSON_AddItemToObject(root, "greeting", str1);
    added2 = cJSON_AddItemToObject(root, "list", arr);
    added3 = cJSON_AddItemToArray(arr, str2);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    is_str = cJSON_IsString(str1);

    // step 5: Validate
    validation_score = (int)added1 + (int)added2 + (int)added3 + (int)is_str + (int)(arr_size == 1);

    // step 6: Cleanup
    (void)validation_score;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}