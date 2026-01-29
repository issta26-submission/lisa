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
//<ID> 1520
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *version_item = NULL;
    cJSON *imported = NULL;
    cJSON *numbers = NULL;
    cJSON *info = NULL;
    cJSON *name = NULL;
    cJSON *count_item = NULL;
    const char *version_str = NULL;
    char json[] = "{ \"numbers\": [10, 20, 30], \"info\": { \"name\": \"TestUser\" } }";
    char prebuf[256];
    memset(prebuf, 0, sizeof(prebuf));
    int array_size = 0;
    char *printed = NULL;

    // step 2: Setup / Configure
    parsed = cJSON_Parse(json);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    version_str = cJSON_Version();
    version_item = cJSON_CreateString(version_str);
    cJSON_AddItemToObject(root, "version", version_item);

    // step 3: Operate / Validate
    imported = cJSON_GetObjectItem(root, "imported");
    numbers = cJSON_GetObjectItem(imported, "numbers");
    array_size = cJSON_GetArraySize(numbers);
    count_item = cJSON_CreateNumber((double)array_size);
    cJSON_AddItemToObject(root, "count", count_item);
    info = cJSON_GetObjectItem(imported, "info");
    name = cJSON_GetObjectItem(info, "name");
    printed = cJSON_PrintUnformatted(root);
    cJSON_PrintPreallocated(root, prebuf, (int)sizeof(prebuf), 0);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}