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
//<ID> 1095
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"orig\",\"count\":2}";
    cJSON *parsed = cJSON_ParseWithLength(json, (size_t)25);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    cJSON_SetValuestring(name_item, "modified");
    cJSON *num = cJSON_AddNumberToObject(root, "added", 3.14);
    cJSON *dup = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "from_parsed", dup);

    // step 3: Operate and Validate
    cJSON *from = cJSON_GetObjectItemCaseSensitive(root, "from_parsed");
    cJSON *count_item = cJSON_GetObjectItemCaseSensitive(from, "count");
    char *printed = cJSON_PrintUnformatted(root);
    char *scratch = (char *)cJSON_malloc(16);
    memset(scratch, 0, 16);
    scratch[0] = printed[0];
    scratch[1] = (char)('0' + (int)count_item->valueint);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}