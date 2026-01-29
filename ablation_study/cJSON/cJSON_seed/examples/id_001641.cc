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
//<ID> 1641
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *manual_str = cJSON_CreateString("generated");
    cJSON_AddItemToObject(root, "manual", manual_str);

    // step 2: Configure
    cJSON_AddStringToObject(meta, "version", "1.2.3");
    cJSON_AddStringToObject(meta, "author", "tester");

    // step 3: Operate & Validate
    cJSON *version_item = cJSON_GetObjectItem(meta, "version");
    const char *ver_str = cJSON_GetStringValue(version_item);
    cJSON *copy_str = cJSON_CreateString(ver_str);
    cJSON_AddItemToObject(root, "version_copy", copy_str);
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);
    char *formatted = cJSON_Print(root);
    cJSON_free(formatted);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}