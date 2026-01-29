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
//<ID> 1359
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *meta = NULL;
    cJSON *version_item = NULL;
    cJSON *number_item = NULL;
    cJSON *flag = NULL;
    char *printed = NULL;
    double retrieved_value = 0.0;
    const char *version = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "count", 100.0);
    meta = cJSON_AddObjectToObject(root, "meta");
    number_item = cJSON_AddNumberToObject(meta, "version_number", 2.71);
    flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", flag);
    version = cJSON_Version();
    cJSON_AddStringToObject(root, "lib_version", version);

    // step 3: Operate / Validate
    retrieved_value = cJSON_GetNumberValue(number_item);
    printed = cJSON_PrintUnformatted(root);
    (void)retrieved_value;
    (void)version;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}