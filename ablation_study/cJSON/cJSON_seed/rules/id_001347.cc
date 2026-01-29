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
//<ID> 1347
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"name\":\"example\",\"values\":[1,2,3]}";
    cJSON *root = NULL;
    cJSON *values = NULL;
    cJSON *name_item = NULL;
    cJSON *first_value = NULL;
    cJSON *true_item = NULL;
    cJSON *dup = NULL;
    char *printed = NULL;
    double first_val = 0.0;
    const char *name_str = NULL;
    int arr_size = 0;
    cJSON_bool eq = 0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", true_item);
    values = cJSON_GetObjectItem(root, "values");
    arr_size = cJSON_GetArraySize(values);

    // step 3: Operate / Validate
    printed = cJSON_PrintBuffered(root, 256, 1);
    name_item = cJSON_GetObjectItem(root, "name");
    name_str = cJSON_GetStringValue(name_item);
    first_value = cJSON_GetArrayItem(values, 0);
    first_val = cJSON_GetNumberValue(first_value);
    dup = cJSON_Duplicate(root, 1);
    eq = cJSON_Compare(root, dup, 1);
    (void)name_str;
    (void)first_val;
    (void)arr_size;
    (void)eq;

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "values");
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}