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
//<ID> 129
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *raw_item = cJSON_CreateRaw("{\"x\":1}");
    cJSON *dup = NULL;
    cJSON *found = NULL;
    char *printed = NULL;
    double extracted = 0.0;
    cJSON_bool is_raw = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure structure
    cJSON_AddItemToObject(child, "value", num_item);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "rawdata", raw_item);
    cJSON_AddNumberToObject(root, "count", 100.0);

    // step 3: Operate / Validate
    found = cJSON_GetObjectItem(root, "count");
    extracted = cJSON_GetNumberValue(found);
    is_raw = cJSON_IsRaw(cJSON_GetObjectItem(root, "rawdata"));
    dup = cJSON_Duplicate(root, 1);
    equal = cJSON_Compare(root, dup, 1);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}