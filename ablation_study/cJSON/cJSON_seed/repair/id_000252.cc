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
//<ID> 252
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
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *names = cJSON_AddArrayToObject(root, "names");
    cJSON *n1 = cJSON_CreateString("Alice");
    cJSON *n2 = cJSON_CreateString("Bob");
    cJSON *n3 = cJSON_CreateString("Carol");
    cJSON_AddItemToArray(names, n1);
    cJSON_AddItemToArray(names, n2);
    cJSON_AddItemToArray(names, n3);
    cJSON_AddItemToObjectCS(meta, "status", cJSON_CreateString("active"));

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(names, 1);
    const char *second_val = cJSON_GetStringValue(second);
    cJSON *picked = cJSON_CreateString(second_val);
    cJSON_AddItemToObjectCS(root, "picked", picked);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}