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
//<ID> 278
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(42.5);
    cJSON *n2 = cJSON_CreateNumber(7.0);
    cJSON *s1 = cJSON_CreateString("hello");
    int arr_size = 0;
    cJSON *first_item = NULL;
    double first_value = 0.0;
    cJSON *derived = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToObject(root, "data", arr);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    first_item = cJSON_GetArrayItem(arr, 0);
    first_value = cJSON_GetNumberValue(first_item);
    derived = cJSON_CreateNumber(first_value + 1.0);
    cJSON_AddItemToObject(root, "first_plus_one", derived);
    printed = cJSON_Print(root);
    cJSON_Minify(printed);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}