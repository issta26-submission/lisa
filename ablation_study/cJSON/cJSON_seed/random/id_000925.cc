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
//<ID> 925
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *str_key = "message";
    const char *flag_key = "enabled";
    const char *str_val = "hello world";
    cJSON *root = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON_bool added_flag = (cJSON_bool)0;
    cJSON_bool has_str = (cJSON_bool)0;
    cJSON_bool has_flag = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and a false boolean item
    root = cJSON_CreateObject();
    false_item = cJSON_CreateFalse();

    // step 3: Configure - add the false item and a string to the root object
    added_flag = cJSON_AddItemToObject(root, flag_key, false_item);
    str_item = cJSON_AddStringToObject(root, str_key, str_val);

    // step 4: Operate - check presence of added items using HasObjectItem
    has_str = cJSON_HasObjectItem(root, str_key);
    has_flag = cJSON_HasObjectItem(root, flag_key);

    // step 5: Validate - compute a simple validation score from results
    validation_score = (int)(root != (cJSON *)0) + (int)(false_item != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)added_flag + (int)has_str + (int)has_flag;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}