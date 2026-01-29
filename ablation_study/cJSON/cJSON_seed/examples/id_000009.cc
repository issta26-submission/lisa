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
//<ID> 9
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
    cJSON *nameStr = cJSON_CreateString("example_name");
    cJSON_bool name_added = cJSON_AddItemToObjectCS(config, "name", nameStr);
    cJSON *versionItem = cJSON_AddNumberToObject(config, "version", 1.2345);

    // step 3: Operate & Validate
    cJSON *retrieved_version = cJSON_GetObjectItem(config, "version");
    double version_value = cJSON_GetNumberValue(retrieved_version);
    (void)version_value;
    (void)name_added;

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(config, "name");
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}