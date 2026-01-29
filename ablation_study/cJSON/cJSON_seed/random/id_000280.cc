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
//<ID> 280
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
    cJSON *num_item = (cJSON *)0;
    cJSON *false_orig = (cJSON *)0;
    cJSON *false_dup = (cJSON *)0;
    cJSON *isfalse_num = (cJSON *)0;
    char json_buf[] = "  {  \"flag\"  : false }  ";
    char *printed = (char *)0;
    cJSON_bool is_false_result = (cJSON_bool)0;
    int arr_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(42.0);
    false_orig = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, num_item);
    false_dup = cJSON_Duplicate(false_orig, 1);
    cJSON_AddItemReferenceToArray(arr, false_dup);
    cJSON_Minify(json_buf);

    // step 4: Operate
    is_false_result = cJSON_IsFalse(false_orig);
    isfalse_num = cJSON_CreateNumber((double)is_false_result);
    cJSON_AddItemToObject(root, "false_as_number", isfalse_num);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 5: Validate
    arr_size = cJSON_GetArraySize(arr);
    cJSON_AddItemToObject(root, "count", cJSON_CreateNumber((double)arr_size));

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(false_orig);

    // API sequence test completed successfully
    return 66;
}