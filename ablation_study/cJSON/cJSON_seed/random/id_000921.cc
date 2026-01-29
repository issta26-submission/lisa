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
//<ID> 921
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *key_name = "name";
    const char *key_flag = "enabled";
    const char *name_val = "example";
    cJSON *root = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON_bool added_flag = (cJSON_bool)0;
    cJSON_bool has_name = (cJSON_bool)0;
    cJSON_bool has_flag = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    false_item = cJSON_CreateFalse();

    // step 3: Configure - add string and false boolean to root
    str_item = cJSON_AddStringToObject(root, key_name, name_val);
    added_flag = cJSON_AddItemToObject(root, key_flag, false_item);

    // step 4: Operate - verify presence of items
    has_name = cJSON_HasObjectItem(root, key_name);
    has_flag = cJSON_HasObjectItem(root, key_flag);

    // step 5: Validate - aggregate simple checks into a score
    validation_score = (int)(root != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)added_flag + (int)has_name + (int)has_flag;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}