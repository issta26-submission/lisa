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
//<ID> 283
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *array = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *got_item = (cJSON *)0;
    cJSON *flag_number = (cJSON *)0;
    char *printed = (char *)0;
    char json_buf[] = "  { \"trim\" : \"   spaced   \" }  ";
    cJSON_bool add_ref_ok = (cJSON_bool)0;
    cJSON_bool is_false_flag = (cJSON_bool)0;
    double flag_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);

    // step 3: Configure
    false_item = cJSON_CreateFalse();
    add_ref_ok = cJSON_AddItemReferenceToArray(array, false_item);
    cJSON_Minify(json_buf);

    // step 4: Operate
    got_item = cJSON_GetArrayItem(array, 0);
    is_false_flag = cJSON_IsFalse(got_item);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    flag_value = (double)is_false_flag;
    flag_number = cJSON_CreateNumber(flag_value);
    cJSON_AddItemToObject(root, "false_flag", flag_number);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}