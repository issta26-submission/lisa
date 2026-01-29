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
//<ID> 849
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("Alice");
    cJSON *lang_item = cJSON_AddStringToObject(root, "language", "C++");

    // step 2: Configure
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON *city_item = cJSON_AddStringToObject(root, "city", "Wonderland");
    cJSON *name_ref = cJSON_CreateStringReference(cJSON_GetStringValue(name_item));
    cJSON_AddItemToObject(root, "name_ref", name_ref);

    // step 3: Operate & Validate
    char *name_val = cJSON_GetStringValue(name_item);
    char *lang_val = cJSON_GetStringValue(lang_item);
    char *city_val = cJSON_GetStringValue(city_item);
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddStringToObject(summary, "name", name_val);
    cJSON_AddStringToObject(summary, "language", lang_val);
    cJSON_AddStringToObject(summary, "city", city_val);
    cJSON_AddItemToObject(root, "summary", summary);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}