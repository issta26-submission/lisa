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
//<ID> 192
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *id = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(root, "id", id);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *v1 = cJSON_CreateNumber(1.5);
    cJSON *v2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(values, v1);
    cJSON_AddItemToArray(values, v2);

    // step 2: Configure
    cJSON *name_replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInObjectCaseSensitive(meta, "name", name_replacement);
    cJSON *id_item = cJSON_GetObjectItemCaseSensitive(root, "id");
    double id_value = cJSON_GetNumberValue(id_item);
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(meta, "name");
    cJSON_bool name_is_string = cJSON_IsString(name_item);

    // step 3: Operate and Validate
    cJSON *flagged = cJSON_CreateNumber(id_value + (double)name_is_string);
    cJSON_AddItemToObject(meta, "flagged_value", flagged);
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_Minify(unformatted);
    void *tmpbuf = cJSON_malloc(128);
    memset(tmpbuf, 0, 128);
    cJSON *parsed = cJSON_Parse(unformatted);
    int summary = (int)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(parsed, "id")) + (int)name_is_string + cJSON_GetArraySize(values);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(tmpbuf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}