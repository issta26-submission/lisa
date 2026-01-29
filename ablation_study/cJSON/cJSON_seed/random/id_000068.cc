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
//<ID> 68
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *child = cJSON_CreateString("child_value");
    cJSON *label_item = cJSON_CreateString("root_label");
    const cJSON *got_label = (cJSON *)0;
    const char *label_value = (const char *)0;

    // step 2: Setup - populate array and attach to root, add a reference to the first child
    cJSON_AddItemToArray(arr, child);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemReferenceToObject(root, "first_child_ref", child);
    cJSON_AddItemToObject(root, "label", label_item);

    // step 3: Operate - read back the label string and echo it into root
    got_label = cJSON_GetObjectItem(root, "label");
    label_value = cJSON_GetStringValue(got_label);
    cJSON_AddStringToObject(root, "echo", label_value);

    // step 4: Validate - create an additional object entry derived from existing data
    cJSON_AddStringToObject(root, "validated_echo", label_value);

    // step 5: Cleanup - remove temporary named entries and then delete the root (which frees remaining items)
    cJSON_DeleteItemFromObject(root, "label");
    cJSON_DeleteItemFromObject(root, "first_child_ref");
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}