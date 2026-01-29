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
//<ID> 785
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"array\": [1, 2, 3], \"keep\": 42}";
    cJSON *root = NULL;
    cJSON *array = NULL;
    cJSON *keep_item = NULL;
    cJSON *new_num = NULL;
    cJSON *added_num = NULL;
    cJSON *replaced_item = NULL;
    char *printed = NULL;
    double keep_value = 0.0;
    double replaced_value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    array = cJSON_GetObjectItem(root, "array");
    keep_item = cJSON_GetObjectItem(root, "keep");
    keep_value = cJSON_GetNumberValue(keep_item);

    // step 3: Operate / Validate
    new_num = cJSON_CreateNumber(99.5);
    cJSON_ReplaceItemInArray(array, 1, new_num);
    added_num = cJSON_CreateNumber(7.7);
    cJSON_AddItemToObject(root, "added", added_num);
    printed = cJSON_PrintUnformatted(root);
    replaced_item = cJSON_GetArrayItem(array, 1);
    replaced_value = cJSON_GetNumberValue(replaced_item);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}