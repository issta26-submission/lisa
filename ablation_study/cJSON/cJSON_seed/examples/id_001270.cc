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
//<ID> 1270
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *items = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");

    // step 2: Configure
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddNullToObject(meta, "optional");
    cJSON_AddItemToObject(root, "title", cJSON_CreateString("example_title"));

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(items);
    cJSON *count_num = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(meta, "count", count_num);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    const char *first_str = cJSON_GetStringValue(first);
    cJSON_AddItemToObject(meta, "first", cJSON_CreateString(first_str ? first_str : ""));
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}