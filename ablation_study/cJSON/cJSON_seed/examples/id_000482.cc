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
//<ID> 482
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
    cJSON *ref = cJSON_CreateStringReference("ref_string");
    cJSON_AddItemToArray(items, ref);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, s1);
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s2);

    // step 2: Configure
    cJSON *first = cJSON_GetArrayItem(items, 0);
    char *first_val = cJSON_GetStringValue(first);
    char *third_val = cJSON_GetStringValue(cJSON_GetArrayItem(items, 2));
    char combined_buf[128];
    memset(combined_buf, 0, sizeof(combined_buf));
    sprintf(combined_buf, "%s+%s", first_val, third_val);

    // step 3: Operate & Validate
    cJSON *combined = cJSON_CreateString(combined_buf);
    cJSON_ReplaceItemViaPointer(items, first, combined);
    cJSON *new_first = cJSON_GetArrayItem(items, 0);
    char *new_first_val = cJSON_GetStringValue(new_first);
    char verify_buf[192];
    memset(verify_buf, 0, sizeof(verify_buf));
    sprintf(verify_buf, "verified:%s", new_first_val);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}