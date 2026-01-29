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
//<ID> 1278
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
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *first = cJSON_CreateString("alpha");
    cJSON *second = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, first);
    cJSON_AddItemToArray(items, second);
    cJSON_AddNullToObject(meta, "optional");

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(meta, "count", (double)count);
    cJSON *title = cJSON_CreateString("example_collection");
    cJSON_AddItemToObject(root, "title", title);
    char *serialized = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(serialized);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}