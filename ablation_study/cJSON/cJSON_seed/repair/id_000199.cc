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
//<ID> 199
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
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(names, s1);
    cJSON_AddItemToArray(names, s2);
    cJSON_AddItemToObject(root, "names", names);
    cJSON_AddStringToObject(root, "greeting", "hello");

    // step 2: Configure
    cJSON *s3 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(names, s3);
    cJSON_AddStringToObject(root, "status", "configured");

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(names);
    cJSON_AddNumberToObject(root, "count", (double)count);
    cJSON *first = cJSON_GetArrayItem(names, 0);
    const char *first_val = cJSON_GetStringValue(first);
    cJSON_AddStringToObject(root, "first_copy", first_val);
    cJSON *second = cJSON_GetArrayItem(names, 1);
    const char *second_val = cJSON_GetStringValue(second);
    cJSON_AddStringToObject(root, "second_copy", second_val);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}