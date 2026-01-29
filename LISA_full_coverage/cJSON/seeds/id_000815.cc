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
//<ID> 815
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *version_item = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(root, "version", version_item);
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON *author_item = cJSON_CreateString("tester");
    cJSON_AddItemToObject(settings, "author", author_item);

    // step 2: Configure
    cJSON_AddStringToObject(settings, "mode", "production");
    cJSON_AddStringToObject(root, "app", "cJSON_test");

    // step 3: Operate
    double newnum = cJSON_SetNumberHelper(version_item, 2.71828);
    double updated = cJSON_SetNumberHelper(version_item, newnum + 1.0);
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    cJSON_free(compact);
    cJSON_free(pretty);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}