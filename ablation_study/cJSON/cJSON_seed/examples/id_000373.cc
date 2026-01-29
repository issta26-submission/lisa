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
//<ID> 373
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON *pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", pi);
    cJSON *maybe = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe);

    // step 2: Configure
    cJSON *greeting_dup = cJSON_Duplicate(greeting, 0);
    cJSON_AddItemToObject(root, "greeting_dup", greeting_dup);
    char *meta = (char *)cJSON_malloc(128);
    memset(meta, 0, 128);

    // step 3: Operate & Validate
    cJSON_bool was_null = cJSON_IsNull(maybe);
    double pi_val = cJSON_GetNumberValue(pi);
    sprintf(meta, "was_null=%d;pi=%.5f", (int)was_null, pi_val);
    cJSON *meta_str = cJSON_CreateString(meta);
    cJSON_AddItemToObject(root, "meta", meta_str);
    cJSON *pi_double = cJSON_CreateNumber(pi_val * 2.0);
    cJSON_AddItemToObject(root, "pi_double", pi_double);

    // step 4: Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_free(meta);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}