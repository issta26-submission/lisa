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
//<ID> 475
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
    cJSON *str_item = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *flag_num = (cJSON *)0;
    const char *sval = (const char *)0;
    cJSON_bool is_raw = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("sample");

    // step 3: Configure
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    item0 = cJSON_GetArrayItem(arr, 0);
    sval = cJSON_GetStringValue(item0);
    is_raw = cJSON_IsRaw(item0);
    cJSON_AddNullToObject(root, sval);

    // step 5: Validate
    flag_num = cJSON_CreateNumber((double)is_raw);
    cJSON_AddItemToObject(root, "raw_flag", flag_num);

    // step 6: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}