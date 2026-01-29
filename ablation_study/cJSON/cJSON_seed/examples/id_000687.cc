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
//<ID> 687
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_CreateArray();
    cJSON *true_item1 = cJSON_CreateTrue();
    cJSON *true_item2 = cJSON_CreateTrue();
    cJSON_AddItemToArray(flags, true_item1);
    cJSON_AddItemToArray(flags, true_item2);
    cJSON_AddItemToObject(root, "flags", flags);
    cJSON_AddNumberToObject(root, "count", 2.0);

    // step 2: Configure
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_flags = cJSON_GetObjectItem(parsed, "flags");
    int parsed_size = cJSON_GetArraySize(parsed_flags);
    cJSON *first = cJSON_GetArrayItem(parsed_flags, 0);
    cJSON *second = cJSON_GetArrayItem(parsed_flags, 1);
    cJSON_bool equal = cJSON_Compare(first, second, 1);
    cJSON_AddBoolToObject(parsed, "elements_equal", equal);
    cJSON_AddNumberToObject(parsed, "size", (double)parsed_size);
    (void)version;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}