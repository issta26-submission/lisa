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
//<ID> 700
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *name_item = cJSON_AddStringToObject(root, "name", "example");
    cJSON *initial_num = cJSON_AddNumberToObject(root, "initial", 42.0);
    cJSON *first_str = cJSON_CreateString("first");
    cJSON_InsertItemInArray(items, 0, first_str);

    // step 2: Configure
    cJSON *name_lookup = cJSON_GetObjectItemCaseSensitive(root, "name");
    cJSON_bool name_is_string = cJSON_IsString(name_lookup);
    char *name_value = cJSON_GetStringValue(name_lookup);
    double name_code = (double)(unsigned char)name_value[0];
    cJSON *name_code_item = cJSON_AddNumberToObject(root, "name_code", name_code);

    // step 3: Operate
    cJSON *pi_item = cJSON_CreateNumber(3.14159);
    cJSON_InsertItemInArray(items, 1, pi_item);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *retrieved_initial = cJSON_GetObjectItemCaseSensitive(root, "initial");
    cJSON *retrieved_name_code = cJSON_GetObjectItemCaseSensitive(root, "name_code");

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}