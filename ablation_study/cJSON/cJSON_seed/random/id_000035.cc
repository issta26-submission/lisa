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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root_obj = cJSON_CreateObject();
    cJSON *root_array = cJSON_CreateArray();
    cJSON *flag_true = cJSON_CreateBool(1);
    cJSON *flag_false = cJSON_CreateBool(0);
    cJSON *detached_flag = (cJSON *)0;
    int remaining_count = 0;
    cJSON_bool moved_present = 0;

    // step 2: Setup
    cJSON_AddItemToArray(root_array, flag_true);
    cJSON_AddItemToArray(root_array, flag_false);
    cJSON_AddItemToObjectCS(root_obj, "flags", root_array);

    // step 3: Configure
    cJSON_AddTrueToObject(root_obj, "guaranteed_true");

    // step 4: Operate
    detached_flag = cJSON_DetachItemFromArray(root_array, 0);
    cJSON_AddItemToObjectCS(root_obj, "moved_flag", detached_flag);

    // step 5: Validate
    remaining_count = cJSON_GetArraySize(root_array);
    moved_present = cJSON_HasObjectItem(root_obj, "moved_flag");

    // step 6: Cleanup
    cJSON_Delete(root_obj);

    // API sequence test completed successfully
    (void)remaining_count;
    (void)moved_present;
    return 66;
}