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
//<ID> 1633
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"flag\": false, \"items\": [1, 2]}";
    cJSON *root = cJSON_Parse(json);
    cJSON *items = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON *flagItem = cJSON_GetObjectItemCaseSensitive(root, "flag");
    cJSON *newTrue = cJSON_CreateBool(1);
    cJSON_AddItemToArray(items, newTrue);

    // step 2: Configure
    cJSON *newFalse = cJSON_CreateBool(0);
    cJSON_AddItemToArray(items, newFalse);
    cJSON_AddNumberToObject(root, "added_number", 42.0);

    // step 3: Operate and Validate
    cJSON_bool flag_true = cJSON_IsTrue(flagItem);
    char *printed = cJSON_PrintBuffered(root, 128, 1);
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (int)flag_true);
    buffer[2] = (char)('0' + (int)cJSON_GetArraySize(items));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}