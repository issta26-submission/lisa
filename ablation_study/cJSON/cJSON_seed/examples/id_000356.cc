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
//<ID> 356
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *labels[] = { "one", "two", "three" };
    cJSON *root = cJSON_CreateObject();
    cJSON *label_array = cJSON_CreateStringArray(labels, 3);
    cJSON_AddItemToObject(root, "labels", label_array);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num, 42.0);
    cJSON_AddItemToObject(root, "answer", num);
    cJSON *extras = cJSON_CreateArray();
    cJSON *extra_item = cJSON_CreateString("extra0");
    cJSON_AddItemToArray(extras, extra_item);
    cJSON_AddItemToObject(root, "extras", extras);
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInArray(label_array, 1, replacement);

    // step 3: Operate & Validate
    cJSON *append_item = cJSON_CreateString("four");
    cJSON_AddItemToArray(label_array, append_item);
    char *snapshot = cJSON_PrintUnformatted(root);
    double updated = cJSON_SetNumberHelper(num, 100.5);
    (void)updated;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}