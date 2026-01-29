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
//<ID> 1350
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *config = NULL;
    cJSON *num_item = NULL;
    cJSON *flag = NULL;
    const char *version = NULL;
    double threshold = 0.0;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    config = cJSON_AddObjectToObject(root, "config");
    num_item = cJSON_AddNumberToObject(config, "threshold", 12.34);
    flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", flag);

    // step 3: Operate / Validate
    version = cJSON_Version();
    threshold = cJSON_GetNumberValue(cJSON_GetObjectItem(config, "threshold"));
    printed = cJSON_PrintUnformatted(root);
    (void)version;
    (void)threshold;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}