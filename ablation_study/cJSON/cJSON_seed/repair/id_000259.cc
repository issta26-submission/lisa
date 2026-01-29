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
//<ID> 259
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

    // step 2: Configure
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON *str2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, str1);
    cJSON_AddItemToArray(items, str2);
    cJSON_AddItemToObjectCS(meta, "creator", cJSON_CreateString("tester"));
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON_AddItemToObjectCS(root, "title", cJSON_CreateString("demo"));

    // step 3: Operate and Validate
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    const char *second_val = cJSON_GetStringValue(second_item);
    cJSON_AddItemToObjectCS(root, "picked", cJSON_CreateString(second_val));
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}