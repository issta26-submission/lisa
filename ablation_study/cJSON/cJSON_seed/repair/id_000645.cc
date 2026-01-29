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
//<ID> 645
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *person = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "person", person);
    cJSON *name_item = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(person, "name", name_item);
    cJSON *active = cJSON_CreateFalse();
    cJSON_AddItemToObject(person, "active", active);
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(person, "age", age);

    // step 2: Configure
    cJSON *retrieved_name_item = cJSON_GetObjectItem(person, "name");
    char *name_str = cJSON_GetStringValue(retrieved_name_item);
    cJSON_AddStringToObject(root, "snapshot_name", name_str);
    cJSON *retrieved_active = cJSON_GetObjectItem(person, "active");
    cJSON_bool was_false = cJSON_IsFalse(retrieved_active);
    cJSON_AddNumberToObject(root, "was_inactive", (double)was_false);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_AddStringToObject(root, "printed", out);
    cJSON_free(out);
    cJSON *dup = cJSON_Duplicate(root, 1);

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}