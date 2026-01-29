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
//<ID> 282
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
    cJSON *got_item = (cJSON *)0;
    cJSON *was_false_item = (cJSON *)0;
    cJSON_bool add_ok_ref = (cJSON_bool)0;
    cJSON_bool is_false_flag = (cJSON_bool)0;
    char *printed = (char *)0;
    char json_buf[] = "  { \"minify_example\" : false }  ";

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    false_item = cJSON_CreateFalse();
    add_ok_ref = cJSON_AddItemReferenceToArray(arr, false_item);
    cJSON_Minify(json_buf);

    // step 4: Operate
    got_item = cJSON_GetArrayItem(arr, 0);
    is_false_flag = cJSON_IsFalse(got_item);

    // step 5: Validate
    was_false_item = cJSON_CreateBool(is_false_flag);
    cJSON_AddItemToObject(root, "was_false", was_false_item);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}