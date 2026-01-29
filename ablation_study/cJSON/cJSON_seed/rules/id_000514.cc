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
//<ID> 514
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"items\":[1,2,3],\"flag\":false}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, (size_t)(sizeof("{\"items\":[1,2,3],\"flag\":false}") - 1), &parse_end, 1);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *flag = cJSON_GetObjectItem(root, "flag");
    cJSON *num42 = cJSON_CreateNumber(42.0);
    cJSON *true_item = cJSON_CreateTrue();
    int size_before = cJSON_GetArraySize(items);
    int size_after = 0;
    cJSON *sumItem = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(items, num42);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", true_item);

    // step 3: Operate / Validate
    size_after = cJSON_GetArraySize(items);
    sumItem = cJSON_CreateNumber((double)(size_before + size_after));
    cJSON_AddItemToObject(root, "sum", sumItem);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}