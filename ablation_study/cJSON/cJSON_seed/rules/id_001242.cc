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
//<ID> 1242
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"name\":\"example\",\"flag\":true,\"value\":123.5,\"items\":[1,2]}";
    cJSON *root = NULL;
    cJSON *flag_item = NULL;
    cJSON *value_item = NULL;
    cJSON *items = NULL;
    cJSON *new_num = NULL;
    char *printed = NULL;
    char prebuf[128];
    cJSON_bool is_true = 0;
    cJSON_bool pre_ok = 0;
    double value_num = 0.0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    flag_item = cJSON_GetObjectItem(root, "flag");
    value_item = cJSON_GetObjectItem(root, "value");
    items = cJSON_GetObjectItem(root, "items");
    memset(prebuf, 0, sizeof(prebuf));

    // step 3: Operate / Validate
    is_true = cJSON_IsTrue(flag_item);
    value_num = cJSON_GetNumberValue(value_item);
    printed = cJSON_PrintUnformatted(root);
    pre_ok = cJSON_PrintPreallocated(root, prebuf, (int)sizeof(prebuf), 0);
    new_num = cJSON_CreateNumber(value_num + 1.0);
    cJSON_AddItemToArray(items, new_num);
    cJSON_AddItemToObject(root, "value_copy", cJSON_CreateNumber(value_num));
    (void)is_true;
    (void)pre_ok;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}