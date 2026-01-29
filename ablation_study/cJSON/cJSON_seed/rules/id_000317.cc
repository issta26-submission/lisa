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
//<ID> 317
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = NULL;
    cJSON *bool_item = NULL;
    cJSON *num_item1 = cJSON_CreateNumber(100.0);
    cJSON *num_item2 = cJSON_CreateNumber(200.0);
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON_bool is_false = 0;
    cJSON_bool is_invalid = 0;
    int arr_size = 0;
    cJSON *retrieved = NULL;
    double retrieved_val = 0.0;

    // step 2: Setup / Configure
    array = cJSON_AddArrayToObject(root, "values");
    cJSON_AddItemToArray(array, num_item1);
    cJSON_AddItemToArray(array, num_item2);
    bool_item = cJSON_AddBoolToObject(root, "enabled", 0);
    cJSON_AddItemToObject(root, "meta", cJSON_CreateObject());

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    is_false = cJSON_IsFalse(bool_item);
    is_invalid = cJSON_IsInvalid(NULL);
    arr_size = cJSON_GetArraySize(array);
    retrieved = cJSON_GetArrayItem(array, 1);
    retrieved_val = cJSON_GetNumberValue(retrieved);
    (void)retrieved_val;
    (void)arr_size;
    (void)is_false;
    (void)is_invalid;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}