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
//<ID> 1536
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *value = NULL;
    cJSON *num = NULL;
    cJSON *dup_value = NULL;
    const char *json = "{\"value\": 3.14, \"keep\": 99}";

    // step 2: Setup / Configure
    parsed = cJSON_Parse(json);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);

    // step 3: Operate / Validate
    value = cJSON_GetObjectItem(parsed, "value");
    num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(parsed, "answer", num);
    dup_value = cJSON_Duplicate(value, 1);
    cJSON_AddItemToObject(root, "value_copy", dup_value);
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "keep");

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}