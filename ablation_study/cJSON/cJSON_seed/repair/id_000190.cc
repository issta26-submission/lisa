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
//<ID> 190
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *names = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "names", names);
    cJSON_AddStringToObject(root, "title", "User List");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("Alice");
    cJSON_AddItemToArray(names, s1);
    cJSON *s2 = cJSON_CreateString("Bob");
    cJSON_AddItemToArray(names, s2);
    cJSON *s3 = cJSON_CreateString("Charlie");
    cJSON_AddItemToArray(names, s3);

    // step 3: Operate and Validate
    int total = cJSON_GetArraySize(names);
    cJSON *count = cJSON_CreateNumber((double)total);
    cJSON_AddItemToObject(root, "count", count);
    cJSON *first_item = cJSON_GetArrayItem(names, 0);
    const char *first_name = cJSON_GetStringValue(first_item);
    cJSON_AddStringToObject(meta, "first", first_name);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}