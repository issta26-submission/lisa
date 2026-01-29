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
//<ID> 289
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[3];
    nums[0] = 1.5;
    nums[1] = 2.5;
    nums[2] = 3.5;
    cJSON *arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON *label = cJSON_CreateString("example_label");

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate & Validate
    cJSON_bool has = cJSON_HasObjectItem(root, "values");
    cJSON *has_item = cJSON_CreateBool(has);
    cJSON_AddItemToObject(root, "has_values", has_item);
    char *printed = cJSON_Print(root);
    char *label_text = cJSON_GetStringValue(label);
    cJSON_AddItemToObject(root, "label_copy", cJSON_CreateString(label_text));
    char scratch[128];
    memset(scratch, 0, sizeof(scratch));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}