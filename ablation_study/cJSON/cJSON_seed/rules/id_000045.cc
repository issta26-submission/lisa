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
//<ID> 45
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
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *s1 = cJSON_CreateString("value");
    cJSON *got_item = NULL;
    cJSON *dup = NULL;
    cJSON *computed_item = NULL;
    char *printed = NULL;
    int arr_size = 0;
    int index_to_get = 1;
    double number_at_index = 0.0;
    double computed_value = 0.0;
    cJSON_bool compare_result = 0;
    cJSON_bool print_fmt = 1;

    // step 2: Setup
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, s1);

    // step 3: Core operations / Validate
    arr_size = cJSON_GetArraySize(arr);
    got_item = cJSON_GetArrayItem(arr, index_to_get);
    number_at_index = cJSON_GetNumberValue(got_item);
    computed_value = number_at_index * (double)arr_size;
    computed_item = cJSON_AddNumberToObject(root, "computed", computed_value);
    dup = cJSON_Duplicate(root, 1);
    compare_result = cJSON_Compare(root, dup, 1);

    // step 4: Cleanup
    printed = cJSON_PrintBuffered(root, 256, print_fmt);
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}