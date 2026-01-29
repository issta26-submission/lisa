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
//<ID> 843
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "version", "1.2.3");
    cJSON *manufacturer_item = cJSON_CreateString("ACME, Inc.");
    cJSON_AddItemToObject(root, "manufacturer", manufacturer_item);
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);

    // step 2: Configure
    cJSON_AddStringToObject(settings, "mode", "auto");
    cJSON *timeout = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(settings, "timeout", timeout);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *ver = cJSON_GetObjectItem(root, "version");
    char *verstr = cJSON_GetStringValue(ver);
    cJSON *man = cJSON_GetObjectItem(root, "manufacturer");
    char *manstr = cJSON_GetStringValue(man);
    cJSON *ver_copy = cJSON_CreateString(verstr);
    cJSON_AddItemToObject(root, "version_copy", ver_copy);
    cJSON *man_copy = cJSON_CreateString(manstr);
    cJSON_AddItemToObject(root, "manufacturer_copy", man_copy);
    char *snapshot2 = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(snapshot2);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}