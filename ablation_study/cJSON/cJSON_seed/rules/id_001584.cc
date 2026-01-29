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
//<ID> 1584
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
    cJSON *arr = NULL;
    cJSON *item0 = NULL;
    cJSON *str_item0 = NULL;
    cJSON *str_item1 = NULL;
    const char *item0_text = NULL;
    char *out = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNumberToObject(meta, "id", 123.0);
    cJSON_AddRawToObject(meta, "config", "{\"enabled\":true,\"threshold\":2.5}");
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    str_item0 = cJSON_CreateString("first");
    str_item1 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, str_item0);
    cJSON_AddItemToArray(arr, str_item1);

    // step 3: Operate / Validate
    item0 = cJSON_GetArrayItem(arr, 0);
    item0_text = cJSON_GetStringValue(item0);
    cJSON_AddItemReferenceToObject(root, "meta_ref", meta);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}