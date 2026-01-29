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
//<ID> 1534
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
    cJSON *num_item = NULL;
    cJSON *keep_item = NULL;
    cJSON *dup_item = NULL;
    cJSON *new_num = NULL;
    char *printed = NULL;
    const char json[] = "{ \"keep\": 123, \"remove\": 456 }";
    double keep_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", num_item);
    parsed = cJSON_Parse(json);
    cJSON_AddItemToObject(root, "imported", parsed);

    // step 3: Operate / Validate
    keep_item = cJSON_GetObjectItem(parsed, "keep");
    dup_item = cJSON_Duplicate(keep_item, 0);
    cJSON_AddItemToObject(root, "dup_keep", dup_item);
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "remove");
    new_num = cJSON_CreateNumber(cJSON_GetNumberValue(keep_item) + 1.0);
    cJSON_AddItemToObject(root, "keep_plus_one", new_num);
    keep_val = cJSON_GetNumberValue(keep_item);
    printed = cJSON_PrintUnformatted(root);
    (void)keep_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}