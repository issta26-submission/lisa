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
//<ID> 816
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
    cJSON *flag = NULL;
    cJSON *found = NULL;
    cJSON *dup_raw = NULL;
    cJSON_bool printed_ok = 0;
    cJSON_bool equal = 0;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    raw = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "meta_raw", raw);
    flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate / Validate
    found = cJSON_GetObjectItemCaseSensitive(root, "meta_raw");
    dup_raw = cJSON_Duplicate(found, 1);
    printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    equal = cJSON_Compare(found, dup_raw, 1);
    (void)printed_ok;
    (void)equal;

    // step 4: Cleanup
    cJSON_Delete(dup_raw);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}