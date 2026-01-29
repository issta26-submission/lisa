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
//<ID> 2034
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", array);
    cJSON *elem0 = cJSON_CreateString("hello");
    cJSON_AddItemToArray(array, elem0);
    cJSON *name_item = cJSON_CreateString("original");
    cJSON_AddItemToObject(root, "name", name_item);

    // step 2: Configure
    cJSON_AddTrueToObject(root, "enabled");
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "name", replacement);

    // step 3: Operate and Validate
    char raw[] = "  { \"note\" : \"trim me\" }  ";
    char jsonbuf[64];
    memset(jsonbuf, 0, sizeof(jsonbuf));
    memcpy(jsonbuf, raw, sizeof(raw));
    cJSON_Minify(jsonbuf);
    cJSON *minroot = cJSON_Parse(jsonbuf);
    char *printed = cJSON_PrintUnformatted(root);
    char *val0 = cJSON_GetStringValue(cJSON_GetArrayItem(array, 0));
    char *note_val = cJSON_GetStringValue(cJSON_GetObjectItem(minroot, "note"));
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = val0[0];
    buffer[2] = note_val[0];
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(minroot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}