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
//<ID> 819
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *meta_raw = NULL;
    cJSON *flag_false = NULL;
    cJSON *dup_root = NULL;
    cJSON *orig_meta = NULL;
    cJSON *dup_meta = NULL;
    cJSON_bool printed = 0;
    cJSON_bool metas_equal = 0;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    meta_raw = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "meta_raw", meta_raw);
    flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag_false);

    // step 3: Operate / Validate
    printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    dup_root = cJSON_Duplicate(root, 1);
    orig_meta = cJSON_GetObjectItemCaseSensitive(root, "meta_raw");
    dup_meta = cJSON_GetObjectItemCaseSensitive(dup_root, "meta_raw");
    metas_equal = cJSON_Compare(orig_meta, dup_meta, 1);

    // step 4: Cleanup
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    (void)printed;
    (void)metas_equal;
    return 66;
}