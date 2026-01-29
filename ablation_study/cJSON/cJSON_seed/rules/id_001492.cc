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
//<ID> 1492
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *parsed = NULL;
    cJSON *child = NULL;
    cJSON *nested_flag_item = NULL;
    char json[] = " [ true, false, { \"inner\": true } ] ";
    char prebuf[256];
    memset(prebuf, 0, sizeof(prebuf));
    cJSON_bool nested_flag_check = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    parsed = cJSON_Parse(json);
    cJSON_AddItemToObject(root, "parsed", parsed);
    child = cJSON_CreateObject();
    cJSON_AddBoolToObject(child, "nested_flag", 0);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddBoolToObject(root, "flag", 1);

    // step 3: Operate / Validate
    cJSON_PrintPreallocated(root, prebuf, (int)sizeof(prebuf), 1);
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);
    nested_flag_item = cJSON_GetObjectItem(child, "nested_flag");
    nested_flag_check = cJSON_IsTrue(nested_flag_item);
    (void)nested_flag_check;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}