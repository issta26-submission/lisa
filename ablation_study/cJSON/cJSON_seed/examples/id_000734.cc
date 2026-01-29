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
//<ID> 734
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *s1 = "alpha";
    const char *s2 = "beta";
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *ref1 = cJSON_CreateStringReference(s1);
    cJSON *ref2 = cJSON_CreateStringReference(s2);

    // step 2: Configure
    cJSON_AddItemToArray(array, ref1);
    cJSON_AddItemToArray(array, ref2);
    cJSON_AddItemToObject(root, "items", array);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *retrieved = cJSON_GetObjectItem(parsed, "items");
    cJSON *first = cJSON_GetArrayItem(retrieved, 0);
    const char *value = cJSON_GetStringValue(first);
    (void)value;
    (void)retrieved;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}