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
//<ID> 1354
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
    cJSON *count_item = NULL;
    cJSON *doubled_item = NULL;
    cJSON *active_item = NULL;
    const char *version = NULL;
    double count_val = 0.0;
    double doubled_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    count_item = cJSON_AddNumberToObject(root, "count", 123.45);
    version = cJSON_Version();
    cJSON_AddStringToObject(root, "version", version);
    active_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", active_item);
    meta = cJSON_AddObjectToObject(root, "meta");

    // step 3: Operate / Validate
    count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    doubled_val = count_val * 2.0;
    doubled_item = cJSON_AddNumberToObject(meta, "doubled", doubled_val);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}