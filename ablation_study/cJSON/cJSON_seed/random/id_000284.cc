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
//<ID> 284
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
    cJSON *false_item = (cJSON *)0;
    cJSON_bool flag0 = (cJSON_bool)0;
    cJSON_bool flag1 = (cJSON_bool)0;
    cJSON_bool both_false = (cJSON_bool)0;
    char json_buf[] = "  { \"note\" : \" trimmed \" , \"valid\" : false }  ";

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemReferenceToArray(arr, false_item);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_Minify(json_buf);

    // step 4: Operate
    flag0 = cJSON_IsFalse(cJSON_GetArrayItem(arr, 0));
    flag1 = cJSON_IsFalse(cJSON_GetArrayItem(arr, 1));
    both_false = (cJSON_bool)(flag0 & flag1);

    // step 5: Validate
    cJSON_AddItemToObject(root, "both_false", cJSON_CreateBool(both_false));

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}