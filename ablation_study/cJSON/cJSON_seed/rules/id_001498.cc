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
//<ID> 1498
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
    cJSON *added_str = NULL;
    cJSON *enabled_item = NULL;
    cJSON *embedded_item = NULL;
    cJSON *num_item = NULL;
    cJSON *first_in_array = NULL;
    cJSON_bool enabled_check = 0;
    cJSON_bool embedded_check = 0;
    double num_value = 0.0;
    char prebuf[256];
    memset(prebuf, 0, sizeof(prebuf));
    const char json[] = " { \"embedded\" : true , \"num\" : 42 } ";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddBoolToObject(root, "enabled", 1);
    parsed = cJSON_Parse(json);
    cJSON_AddItemToObject(root, "from_json", parsed);
    added_str = cJSON_CreateString("first-element");
    cJSON_AddItemToArray(arr, added_str);

    // step 3: Operate / Validate
    enabled_item = cJSON_GetObjectItem(root, "enabled");
    enabled_check = cJSON_IsTrue(enabled_item);
    embedded_item = cJSON_GetObjectItem(parsed, "embedded");
    embedded_check = cJSON_IsTrue(embedded_item);
    num_item = cJSON_GetObjectItem(parsed, "num");
    num_value = cJSON_GetNumberValue(num_item);
    first_in_array = cJSON_GetArrayItem(arr, 0);
    cJSON_PrintPreallocated(root, prebuf, (int)sizeof(prebuf), 1);
    (void)enabled_check;
    (void)embedded_check;
    (void)num_value;
    (void)first_in_array;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}