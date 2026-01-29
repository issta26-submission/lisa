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
//<ID> 1352
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *number_item = NULL;
    cJSON *meta_obj = NULL;
    cJSON *flag_item = NULL;
    const char *version = NULL;
    double extracted_value = 0.0;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    number_item = cJSON_AddNumberToObject(root, "count", 123.0);
    meta_obj = cJSON_AddObjectToObject(root, "meta");
    flag_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta_obj, "enabled", flag_item);
    version = cJSON_Version();
    cJSON_AddStringToObject(root, "version", version);

    // step 3: Operate / Validate
    extracted_value = cJSON_GetNumberValue(number_item);
    printed = cJSON_PrintUnformatted(root);
    (void)extracted_value;
    (void)version;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}