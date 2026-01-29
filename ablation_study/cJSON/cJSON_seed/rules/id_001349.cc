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
//<ID> 1349
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *name = NULL;
    cJSON *count = NULL;
    cJSON *flag = NULL;
    const char *name_str = NULL;
    double count_val = 0.0;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name);
    count = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "count", count);
    flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", flag);
    cJSON_AddItemToObject(root, "temp", cJSON_CreateString("delete_me"));

    // step 3: Operate / Validate
    name_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    printed = cJSON_PrintBuffered(root, 256, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "temp");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}