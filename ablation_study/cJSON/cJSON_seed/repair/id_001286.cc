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
//<ID> 1286
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("Alice");
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(config, "name", name);
    cJSON_AddItemToObject(config, "age", age);
    cJSON_AddItemToObject(root, "config", config);
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(3.0));
    cJSON_AddItemToObject(root, "numbers", numbers);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(config, "name");
    cJSON *new_name = cJSON_CreateString("Bob");
    cJSON_ReplaceItemViaPointer(config, name_item, new_name);

    // step 3: Operate and Validate
    cJSON *cfg = cJSON_GetObjectItem(root, "config");
    cJSON *age_item = cJSON_GetObjectItem(cfg, "age");
    double age_value = cJSON_GetNumberValue(age_item);
    cJSON *age_double = cJSON_CreateNumber(age_value * 2.0);
    cJSON_AddItemToObject(root, "age_double", age_double);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}