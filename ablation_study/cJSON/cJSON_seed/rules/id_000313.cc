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
//<ID> 313
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = NULL;
    cJSON *bool_item = NULL;
    cJSON *num_item1 = cJSON_CreateNumber(42.0);
    cJSON *num_item2 = cJSON_CreateNumber(7.5);
    cJSON *str_item = cJSON_CreateString("hello");
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON_bool is_false = 0;
    cJSON_bool is_invalid = 0;
    int arr_size = 0;
    double first_val = 0.0;
    const char *str_val = NULL;
    cJSON *retrieved_bool = NULL;
    cJSON *first_elem = NULL;

    // step 2: Setup / Configure
    bool_item = cJSON_AddBoolToObject(root, "enabled", 0); // add a false boolean
    arr = cJSON_AddArrayToObject(root, "items"); // create and add array to root
    cJSON_AddItemToArray(arr, num_item1);
    cJSON_AddItemToArray(arr, num_item2);
    cJSON_AddItemToObject(root, "message", str_item);

    // step 3: Operate / Validate
    retrieved_bool = cJSON_GetObjectItem(root, "enabled");
    is_false = cJSON_IsFalse(retrieved_bool);
    is_invalid = cJSON_IsInvalid(NULL);
    arr_size = cJSON_GetArraySize(arr);
    first_elem = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetNumberValue(first_elem);
    str_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "message"));
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)is_false;
    (void)is_invalid;
    (void)arr_size;
    (void)first_val;
    (void)str_val;
    return 66;
}