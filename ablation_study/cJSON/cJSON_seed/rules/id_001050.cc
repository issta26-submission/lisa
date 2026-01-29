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
//<ID> 1050
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *sub = NULL;
    cJSON *inner = NULL;
    cJSON *inner_ref = NULL;
    cJSON *sub_ref = NULL;
    cJSON *sub_dup = NULL;
    cJSON *inner_dup = NULL;
    cJSON *flag = NULL;
    cJSON *new_sub = NULL;
    cJSON *got_sub = NULL;
    char *printed = NULL;
    const char *version = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    sub = cJSON_CreateObject();
    inner = cJSON_CreateString("inner_value");
    cJSON_AddItemToObject(sub, "inner", inner);
    cJSON_AddItemToObject(root, "sub", sub);
    flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", flag);

    // To avoid potential double-free issues when replacing the original 'sub',
    // create duplicates that will be referenced by the reference items.
    sub_dup = cJSON_Duplicate(sub, 1);
    sub_ref = cJSON_CreateObjectReference(sub_dup);
    cJSON_AddItemToObject(root, "sub_ref", sub_ref);
    inner_dup = cJSON_Duplicate(inner, 0);
    inner_ref = cJSON_CreateObjectReference(inner_dup);
    cJSON_AddItemToObject(root, "inner_ref", inner_ref);

    version = cJSON_Version();
    cJSON_AddStringToObject(root, "lib_version", version);

    // step 3: Operate / Validate
    got_sub = cJSON_GetObjectItemCaseSensitive(root, "sub");
    new_sub = cJSON_CreateObject();
    cJSON_AddStringToObject(new_sub, "replaced", "yes");
    cJSON_ReplaceItemViaPointer(root, got_sub, new_sub);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
}