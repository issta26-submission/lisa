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
//<ID> 937
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
    cJSON *name = cJSON_CreateString("sensor-alpha");
    cJSON *raw_meta = cJSON_CreateRaw("{\"created\":true,\"v\":1}");
    cJSON *id = cJSON_CreateNumber(100.0);

    // step 2: Configure
    cJSON *dup_name = cJSON_Duplicate(name, 1);
    cJSON_AddItemToObject(person, "name", dup_name);
    cJSON_AddItemToObject(person, "id", id);
    cJSON_AddItemToObject(root, "device", person);
    cJSON_AddItemToObject(root, "metadata", raw_meta);
    cJSON_AddItemToObject(root, "original_name", name);

    // step 3: Operate & Validate
    cJSON_bool name_is_string = cJSON_IsString(name);
    cJSON *is_string_flag = cJSON_CreateBool(name_is_string);
    cJSON_AddItemToObject(root, "name_is_string", is_string_flag);
    char *json_text = cJSON_PrintUnformatted(root);
    cJSON *root_copy = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, root_copy, 1);
    (void)equal;
    cJSON_free(json_text);
    cJSON_Delete(root_copy);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}