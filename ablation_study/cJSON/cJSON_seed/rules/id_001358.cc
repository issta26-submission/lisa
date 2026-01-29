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
//<ID> 1358
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
    cJSON *top_item = NULL;
    cJSON *flag_item = NULL;
    double count_val = 0.0;
    double top_val = 0.0;
    const char *version = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    meta = cJSON_AddObjectToObject(root, "meta");
    count_item = cJSON_AddNumberToObject(meta, "count", 100.0);
    flag_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "enabled", flag_item);
    top_item = cJSON_AddNumberToObject(root, "threshold", 3.5);

    // step 3: Operate / Validate
    count_val = cJSON_GetNumberValue(count_item);
    top_val = cJSON_GetNumberValue(top_item);
    version = cJSON_Version();
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)count_val;
    (void)top_val;
    (void)version;
    return 66;
}