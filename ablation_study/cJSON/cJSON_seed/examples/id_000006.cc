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
//<ID> 6
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations (Initialize)
    cJSON *root = cJSON_CreateObject();
    cJSON *config = NULL;
    cJSON *name_item = NULL;
    cJSON *obsolete_item = NULL;
    cJSON *retrieved_config = NULL;
    cJSON *version_item = NULL;
    double version_value = 0.0;

    // step 2: Setup (Configure)
    config = cJSON_AddObjectToObject(root, "config");
    name_item = cJSON_CreateString("example");
    cJSON_AddItemToObjectCS(config, "name", name_item);
    cJSON_AddNumberToObject(config, "version", 1.23);

    // step 3: Operate and Validate (Operate → Validate)
    obsolete_item = cJSON_CreateString("temporary");
    cJSON_AddItemToObjectCS(root, "obsolete", obsolete_item);
    cJSON_DeleteItemFromObject(root, "obsolete");
    retrieved_config = cJSON_GetObjectItem(root, "config");
    version_item = cJSON_GetObjectItem(retrieved_config, "version");
    version_value = cJSON_GetNumberValue(version_item);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)version_value;
    return 66;
}