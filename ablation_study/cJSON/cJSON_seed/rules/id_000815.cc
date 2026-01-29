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
//<ID> 815
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *raw_item = NULL;
    cJSON *false_item = NULL;
    cJSON *retrieved_raw = NULL;
    cJSON *dup_root = NULL;
    char buffer[256];
    cJSON_bool printed_ok = 0;
    cJSON_bool cmp_result = 0;
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    raw_item = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "meta_raw", raw_item);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", false_item);

    // step 3: Operate / Validate
    printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    retrieved_raw = cJSON_GetObjectItemCaseSensitive(root, "meta_raw");
    dup_root = cJSON_Duplicate(root, 1);
    cmp_result = cJSON_Compare(root, dup_root, 1);

    // step 4: Cleanup
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)buffer;
    (void)printed_ok;
    (void)retrieved_raw;
    (void)cmp_result;
    return 66;
}