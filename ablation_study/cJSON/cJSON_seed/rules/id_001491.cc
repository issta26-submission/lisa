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
//<ID> 1491
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
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *found = NULL;
    cJSON_bool enabled = 0;
    char json[] = " { \"name\": \"example\", \"value\": 42, \"flag\": false } ";
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddBoolToObject(root, "enabled", 1);
    item0 = cJSON_CreateString("first");
    cJSON_AddItemToArray(arr, item0);
    item1 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, item1);

    // step 3: Operate / Validate
    parsed = cJSON_Parse(json);
    cJSON_AddItemToObject(root, "parsed", parsed);
    found = cJSON_GetObjectItem(parsed, "value");
    double val = cJSON_GetNumberValue(found);
    enabled = cJSON_IsTrue(cJSON_GetObjectItem(root, "enabled"));
    (void)val;
    (void)enabled;
    (void)buffer;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}