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
//<ID> 253
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *names = cJSON_AddArrayToObject(root, "names");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta", meta);

    // step 2: Configure
    cJSON *n0 = cJSON_CreateString("alice");
    cJSON *n1 = cJSON_CreateString("bob");
    cJSON *n2 = cJSON_CreateString("carol");
    cJSON_AddItemToArray(names, n0);
    cJSON_AddItemToArray(names, n1);
    cJSON_AddItemToArray(names, n2);
    cJSON_AddItemToObjectCS(meta, "source", cJSON_CreateString("generated"));

    // step 3: Operate and Validate
    cJSON *first_item = cJSON_GetArrayItem(names, 0);
    const char *first_name = cJSON_GetStringValue(first_item);
    cJSON_AddStringToObject(meta, "first", first_name);
    cJSON_AddStringToObject(root, "primary_name", first_name);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}