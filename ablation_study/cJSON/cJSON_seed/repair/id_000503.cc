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
//<ID> 503
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_AddNumberToObject(root, "count", 7.0);
    cJSON *label = cJSON_CreateString("initial_label");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    cJSON *old_label = cJSON_GetObjectItem(root, "label");
    cJSON *new_label = cJSON_CreateString("updated_label");
    cJSON_ReplaceItemViaPointer(root, old_label, new_label);

    // step 3: Operate
    char *serialized = cJSON_PrintUnformatted(root);
    cJSON_free(serialized);

    // step 4: Validate and Cleanup
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_value = cJSON_GetNumberValue(count_item);
    (void)count_value;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}