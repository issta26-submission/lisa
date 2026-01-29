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
//<ID> 811
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *raw = NULL;
    cJSON *false_item = NULL;
    cJSON *duplicate = NULL;
    cJSON *meta_item = NULL;
    cJSON *flag_item = NULL;
    cJSON_bool printed_ok = 0;
    cJSON_bool equal = 0;
    char buffer[256];
    const char *raw_text = "{\"meta\":true}";
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    raw = cJSON_CreateRaw(raw_text);
    cJSON_AddItemToObject(root, "meta_raw", raw);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(child, "flag", false_item);
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate / Validate
    meta_item = cJSON_GetObjectItemCaseSensitive(root, "meta_raw");
    flag_item = cJSON_GetObjectItemCaseSensitive(child, "flag");
    printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    duplicate = cJSON_Duplicate(root, 1);
    equal = cJSON_Compare(root, duplicate, 1);
    (void)meta_item;
    (void)flag_item;
    (void)printed_ok;
    (void)equal;

    // step 4: Cleanup
    cJSON_Delete(duplicate);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}