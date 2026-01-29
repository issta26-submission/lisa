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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = cJSON_AddObjectToObject(root, "nested");
    // step 2: Configure
    cJSON *label = cJSON_CreateString("example label");
    cJSON_AddItemToObjectCS(nested, "label", label);
    cJSON_AddNumberToObject(nested, "count", 42.0);
    // step 3: Operate & Validate
    cJSON *numItem = cJSON_GetObjectItem(nested, "count");
    double value = cJSON_GetNumberValue(numItem);
    cJSON_SetNumberHelper(numItem, value + 1.0);
    double newValue = cJSON_GetNumberValue(numItem);
    cJSON_AddNumberToObject(root, "total", newValue);
    // step 4: Cleanup
    cJSON_DeleteItemFromObject(nested, "label");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}