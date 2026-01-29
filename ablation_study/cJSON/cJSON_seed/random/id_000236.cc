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
//<ID> 236
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
    cJSON *item_str = (cJSON *)0;
    cJSON *item_num = (cJSON *)0;
    cJSON *duparr = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON_bool is_arr = (cJSON_bool)0;
    cJSON_bool cmp_equal = (cJSON_bool)0;
    int array_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    item_str = cJSON_CreateString("first");
    item_num = cJSON_CreateNumber(42.0);

    // step 4: Operate
    add_ok1 = cJSON_AddItemToArray(arr, item_str);
    add_ok2 = cJSON_AddItemToArray(arr, item_num);
    is_arr = cJSON_IsArray(arr);
    cJSON_AddItemToObject(root, "is_array", cJSON_CreateBool(is_arr));
    duparr = cJSON_Duplicate(arr, (cJSON_bool)1);
    cmp_equal = cJSON_Compare(arr, duparr, (cJSON_bool)1);
    cJSON_AddItemToObject(root, "array_equal", cJSON_CreateBool(cmp_equal));

    // step 5: Validate
    array_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)array_size);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(duparr);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}