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
//<ID> 928
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *key_name = "name";
    const char *key_flag = "flag";
    const char *name_val = "tester";
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    const char *extracted = (const char *)0;
    cJSON_bool has_name = (cJSON_bool)0;
    cJSON_bool is_false = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    str_item = cJSON_AddStringToObject(root, key_name, name_val);
    cJSON_AddItemToObject(root, key_flag, false_item);

    // step 4: Operate
    has_name = cJSON_HasObjectItem(root, key_name);
    extracted = cJSON_GetStringValue(str_item);
    is_false = cJSON_IsFalse(false_item);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)has_name + (int)(extracted != (const char *)0) + (int)is_false;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}