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
//<ID> 43
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_Parse("{\"existing\": 7}");
    // step 2: Configure
    const double nums[2] = {3.14, 2.718};
    cJSON *dbl_array = cJSON_CreateDoubleArray(nums, 2);
    cJSON_bool added_array = cJSON_AddItemToObjectCS(root, "doubles", dbl_array);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_bool added_false = cJSON_AddItemToObjectCS(root, "active", false_item);
    // step 3: Operate & Validate
    cJSON *existing_item = cJSON_GetObjectItem(root, "existing");
    double existing_val = cJSON_GetNumberValue(existing_item);
    cJSON *double_existing = cJSON_CreateNumber(existing_val * 2.0);
    cJSON_bool added_double_existing = cJSON_AddItemToObjectCS(root, "double_existing", double_existing);
    cJSON_bool has_existing = cJSON_HasObjectItem(root, "existing");
    cJSON_bool has_doubles = cJSON_HasObjectItem(root, "doubles");
    (void)added_array;
    (void)added_false;
    (void)added_double_existing;
    (void)has_existing;
    (void)has_doubles;
    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}