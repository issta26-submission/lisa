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
//<ID> 392
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *threshold = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(settings, "threshold", threshold);
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddItemReferenceToObject(root, "settings_ref", settings);
    cJSON *temp_value = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "temp", temp_value);
    char *printed_full = cJSON_PrintBuffered(root, 256, 1);
    char *printed_compact = cJSON_PrintBuffered(root, 64, 0);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "temp");
    char *printed_after_delete = cJSON_PrintBuffered(root, 128, 0);
    cJSON_free(printed_full);
    cJSON_free(printed_compact);
    cJSON_free(printed_after_delete);
    cJSON_Delete(root);
    return 66;
}