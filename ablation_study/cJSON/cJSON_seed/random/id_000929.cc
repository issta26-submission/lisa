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
//<ID> 929
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *flag_key = "flag";
    const char *name_key = "name";
    const char *name_val = "example_name";
    cJSON *root = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON_bool has_name = (cJSON_bool)0;
    cJSON_bool has_flag = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object
    root = cJSON_CreateObject();

    // step 3: Configure - create a false boolean and attach to root
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, flag_key, false_item);

    // step 4: Operate - add a string to the root using AddStringToObject
    str_item = cJSON_AddStringToObject(root, name_key, name_val);

    // step 5: Validate - check presence of items and aggregate simple score
    has_name = cJSON_HasObjectItem(root, name_key);
    has_flag = cJSON_HasObjectItem(root, flag_key);
    validation_score = (int)(root != (cJSON *)0) + (int)(false_item != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)has_name + (int)has_flag;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}