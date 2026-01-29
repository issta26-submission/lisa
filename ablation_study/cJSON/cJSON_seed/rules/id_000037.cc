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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON *s3 = cJSON_CreateString("three");
    cJSON *count_item = NULL;
    cJSON *double_item = NULL;
    cJSON *retrieved_count = NULL;
    cJSON *retrieved_array = NULL;
    cJSON *first = NULL;
    cJSON *first_copy = NULL;
    char *printed = NULL;
    int array_size = 0;
    double count_value = 0.0;
    double double_value = 0.0;

    // step 2: Setup
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, s3);

    // step 3: Operate / Validate
    array_size = cJSON_GetArraySize(arr);
    count_item = cJSON_AddNumberToObject(root, "count", (double)array_size);
    retrieved_count = cJSON_GetObjectItem(root, "count");
    count_value = cJSON_GetNumberValue(retrieved_count);
    double_value = count_value * 2.0;
    double_item = cJSON_AddNumberToObject(root, "double_count", double_value);
    retrieved_array = cJSON_GetObjectItem(root, "items");
    first = cJSON_GetArrayItem(retrieved_array, 0);
    first_copy = cJSON_CreateString(cJSON_GetStringValue(first) ? cJSON_GetStringValue(first) : "");
    cJSON_AddItemToArray(arr, first_copy);

    // step 4: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}