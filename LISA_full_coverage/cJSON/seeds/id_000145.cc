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
//<ID> 145
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize and Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("synth");
    cJSON *version = cJSON_CreateNumber(1.0);
    cJSON_bool added_name = cJSON_AddItemToObjectCS(info, "name", name);
    cJSON_bool added_version = cJSON_AddItemToObjectCS(info, "version", version);
    cJSON_bool added_info = cJSON_AddItemToObjectCS(root, "info", info);
    cJSON *items = cJSON_CreateArray();
    cJSON_bool added_items = cJSON_AddItemToObjectCS(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(42.0));
    cJSON_AddItemToArray(items, cJSON_CreateString("alpha"));
    cJSON *maybe = cJSON_CreateNull();
    cJSON_bool added_maybe = cJSON_AddItemToObjectCS(root, "maybe", maybe);

    // step 2: Operate
    cJSON *info_ptr = cJSON_GetObjectItemCaseSensitive(root, "info");
    cJSON *name_ptr = cJSON_GetObjectItemCaseSensitive(info_ptr, "name");
    cJSON *version_ptr = cJSON_GetObjectItemCaseSensitive(info_ptr, "version");
    char *printed = cJSON_PrintBuffered(root, 128, 1);

    // step 3: Validate
    cJSON *maybe_ptr = cJSON_GetObjectItemCaseSensitive(root, "maybe");
    cJSON_bool is_null = cJSON_IsNull(maybe_ptr);
    int summary = (printed ? (int)printed[0] : 0) + (int)is_null + (int)added_name + (int)added_version + (int)added_info + (int)added_items + (info_ptr ? info_ptr->type : 0) + (items ? items->type : 0);
    (void)name_ptr;
    (void)version_ptr;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}