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
//<ID> 926
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *name_key = "name";
    const char *flag_key = "flag";
    const char *tags_key = "tags";
    const char *name_val = "testname";
    const char *tag0_val = "tag1";
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *tags_arr = (cJSON *)0;
    cJSON *tag0 = (cJSON *)0;
    cJSON_bool has_flag = (cJSON_bool)0;
    const char *extracted_name = (const char *)0;
    int validation_score = 0;

    // step 2: Setup - create root, array and items
    root = cJSON_CreateObject();
    tags_arr = cJSON_CreateArray();
    tag0 = cJSON_CreateString(tag0_val);

    // step 3: Configure - attach array and items, add string and false using required APIs
    cJSON_AddItemToArray(tags_arr, tag0);
    cJSON_AddItemToObject(root, tags_key, tags_arr);
    name_item = cJSON_AddStringToObject(root, name_key, name_val);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, flag_key, false_item);

    // step 4: Operate - query presence and extract values
    has_flag = cJSON_HasObjectItem(root, flag_key);
    extracted_name = cJSON_GetStringValue(cJSON_GetObjectItem(root, name_key));

    // step 5: Validate - aggregate simple checks into a score
    validation_score = (int)(root != (cJSON *)0) + (int)(name_item != (cJSON *)0) + (int)has_flag + (int)(extracted_name != (const char *)0 && extracted_name[0] == name_val[0]);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}