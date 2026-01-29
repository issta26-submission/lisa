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
//<ID> 703
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *names = cJSON_CreateArray();
    cJSON *item_a = cJSON_CreateString("alpha");
    cJSON *item_b = cJSON_CreateString("beta");
    cJSON_InsertItemInArray(names, 0, item_a);
    cJSON_InsertItemInArray(names, 1, item_b);
    cJSON_AddItemToObject(root, "names", names);

    // step 2: Configure
    cJSON *metrics = cJSON_AddObjectToObject(root, "metrics");
    int names_count = cJSON_GetArraySize(names);
    cJSON_AddNumberToObject(metrics, "count", (double)names_count);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(names, 0);
    cJSON_bool first_is_string = cJSON_IsString(first);
    char *first_text = cJSON_GetStringValue(first);
    cJSON *count_item = cJSON_GetObjectItemCaseSensitive(metrics, "count");
    double count_value = cJSON_GetNumberValue(count_item);
    cJSON_AddNumberToObject(root, "validated_count", count_value);
    cJSON_AddNumberToObject(metrics, "first_is_string", (double)first_is_string);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}