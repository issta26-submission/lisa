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
//<ID> 765
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);
    cJSON *enabled_item = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *fetched_enabled = cJSON_GetObjectItemCaseSensitive(root, "enabled");
    cJSON *enabled_ref = cJSON_Duplicate(fetched_enabled, 1);
    cJSON_AddItemToObject(root, "enabled_ref", enabled_ref);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
}