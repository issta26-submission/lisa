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
//<ID> 288
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
    cJSON *arr_copy = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool ref_added = (cJSON_bool)0;
    cJSON_bool is_false_flag = (cJSON_bool)0;
    char json_buf[] = "  { \"data\" : false }  ";

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    bool_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, bool_item);
    cJSON_AddItemToObject(root, "original", arr);

    // step 3: Configure
    arr_copy = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "copy", arr_copy);
    ref_added = cJSON_AddItemReferenceToArray(arr_copy, bool_item);
    cJSON_AddNumberToObject(root, "ref_added_flag", (double)ref_added);

    // step 4: Operate
    is_false_flag = cJSON_IsFalse(bool_item);
    num_item = cJSON_CreateNumber((double)is_false_flag);
    cJSON_AddItemToObject(root, "is_false_numeric", num_item);
    cJSON_Minify(json_buf);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}