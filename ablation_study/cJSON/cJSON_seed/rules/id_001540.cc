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
//<ID> 1540
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
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *arr_ref = NULL;
    cJSON *name = NULL;
    cJSON *active = NULL;
    char *printed = NULL;
    int arr_size = 0;
    double first_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name);
    arr = cJSON_AddArrayToObject(root, "values");
    num0 = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(arr, num0);
    num1 = cJSON_CreateNumber(20.25);
    cJSON_AddItemToArray(arr, num1);
    arr_ref = cJSON_Duplicate(arr, 1); /* duplicate array instead of creating a reference to avoid double-free issues */
    cJSON_AddItemToObject(root, "values_ref", arr_ref);
    active = cJSON_AddTrueToObject(root, "active");

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    cJSON_AddNumberToObject(root, "count", (double)arr_size);
    cJSON_AddNumberToObject(root, "first_times_two", first_val * 2.0);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    return 66;
}