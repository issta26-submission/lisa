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
//<ID> 818
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *raw = NULL;
    cJSON *fals = NULL;
    cJSON *dup_raw = NULL;
    cJSON *fetched_raw = NULL;
    cJSON *fetched_false = NULL;
    cJSON *bool_cmp = NULL;
    char buffer[256];
    cJSON_bool printed_ok = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    raw = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "meta_raw", raw);
    fals = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "is_active", fals);

    // step 3: Operate / Validate
    memset(buffer, 0, sizeof(buffer));
    printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    fetched_raw = cJSON_GetObjectItemCaseSensitive(root, "meta_raw");
    fetched_false = cJSON_GetObjectItemCaseSensitive(root, "is_active");
    dup_raw = cJSON_Duplicate(fetched_raw, 1);
    equal = cJSON_Compare(fetched_raw, dup_raw, 1);
    bool_cmp = cJSON_CreateBool(equal);
    cJSON_AddItemToObject(root, "raw_equal", bool_cmp);

    // step 4: Cleanup
    cJSON_Delete(dup_raw);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)printed_ok;
    (void)fetched_false;
    (void)equal;
    return 66;
}