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
//<ID> 1419
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *template_obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(template_obj, "v", 7.0);
    cJSON_AddItemToObject(root, "template", template_obj);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);

    // step 3: Operate & Validate
    cJSON *tmpl = cJSON_GetObjectItem(root, "template");
    cJSON_AddItemReferenceToArray(array, tmpl);
    cJSON *dup = cJSON_Duplicate(tmpl, 1);
    cJSON_AddItemToArray(array, dup);
    cJSON *ver_item = cJSON_GetObjectItem(root, "version");
    double ver = cJSON_GetNumberValue(ver_item);
    cJSON_SetNumberHelper(ver_item, ver + 1.0);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddRawToObject(root, "snapshot", snapshot);
    cJSON_free((void *)snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}