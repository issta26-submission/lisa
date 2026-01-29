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
//<ID> 45
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"initial\": 7.5}";
    cJSON *root = cJSON_Parse(json_text);
    double nums[3];
    nums[0] = 1.1;
    nums[1] = 2.2;
    nums[2] = 3.3;
    cJSON *double_array = cJSON_CreateDoubleArray(nums, 3);
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_bool added_doubles = cJSON_AddItemToObjectCS(root, "doubles", double_array);
    cJSON_bool added_enabled = cJSON_AddItemToObjectCS(root, "enabled", false_item);

    // step 3: Operate & Validate
    cJSON_bool has_initial = cJSON_HasObjectItem(root, "initial");
    cJSON *has_initial_item = cJSON_CreateBool(has_initial);
    cJSON_bool added_has_initial = cJSON_AddItemToObjectCS(root, "has_initial", has_initial_item);
    cJSON_bool has_doubles = cJSON_HasObjectItem(root, "doubles");
    cJSON *has_doubles_item = cJSON_CreateBool(has_doubles);
    cJSON_bool added_has_doubles = cJSON_AddItemToObjectCS(root, "has_doubles", has_doubles_item);
    (void)added_doubles;
    (void)added_enabled;
    (void)added_has_initial;
    (void)added_has_doubles;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}