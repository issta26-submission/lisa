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
//<ID> 7
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    // step 2: Configure
    cJSON *nameItem = cJSON_CreateString("example-name");
    cJSON_AddItemToObjectCS(config, "name", nameItem);
    cJSON *numberItem = cJSON_AddNumberToObject(config, "threshold", 3.1415);
    // step 3: Operate and Validate
    double threshold_value = cJSON_GetNumberValue(numberItem);
    (void)threshold_value; // use the value to show meaningful data flow
    cJSON_DeleteItemFromObject(config, "name");
    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}