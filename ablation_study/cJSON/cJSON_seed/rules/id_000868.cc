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
//<ID> 868
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *str_ref = NULL;
    cJSON *num_created = NULL;
    cJSON *obj = NULL;
    cJSON *num_added = NULL;
    char *printed = NULL;
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool is_array = 0;
    const char *str_val = NULL;
    double num_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    str_ref = cJSON_CreateStringReference("static_ref");
    cJSON_AddItemToArray(arr, str_ref);
    num_created = cJSON_CreateNumber(7.0);
    cJSON_InsertItemInArray(arr, 0, num_created);
    obj = cJSON_CreateObject();
    num_added = cJSON_AddNumberToObject(obj, "count", 42.0);
    cJSON_AddItemToArray(arr, obj);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    is_array = cJSON_IsArray(arr);
    str_val = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 1));
    num_val = cJSON_GetNumberValue(cJSON_GetObjectItem(obj, "count"));
    buffer[0] = (is_array ? 'A' : 'N');
    (void)str_val;
    (void)num_added;
    (void)num_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}