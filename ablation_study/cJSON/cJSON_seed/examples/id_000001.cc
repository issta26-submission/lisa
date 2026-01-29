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
//<ID> 1
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");

    // step 2: Configure
    cJSON *name_item = cJSON_CreateString("example_name");
    cJSON_AddItemToObjectCS(settings, "name", name_item);
    cJSON_AddNumberToObject(settings, "threshold", 42.0);

    // step 3: Operate and Validate
    cJSON *threshold_item = cJSON_GetObjectItem(settings, "threshold");
    double threshold_value = cJSON_GetNumberValue(threshold_item);
    cJSON_AddNumberToObject(root, "threshold_copy", threshold_value + 1.0);

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(settings, "name");
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}