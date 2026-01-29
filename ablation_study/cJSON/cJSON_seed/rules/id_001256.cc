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
//<ID> 1256
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const double numbers[3] = { 1.5, 2.5, 3.5 };
    cJSON *root = NULL;
    cJSON *values_arr = NULL;
    cJSON *retrieved_arr = NULL;
    cJSON *second_elem = NULL;
    cJSON *tmp = NULL;
    char *printed = NULL;
    double second_value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    values_arr = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "values", values_arr);
    cJSON_AddStringToObject(root, "description", "double array example");
    cJSON_AddNullToObject(root, "optional");
    cJSON_AddRawToObject(root, "metadata", "{\"source\":\"generated\",\"valid\":true}");

    // step 3: Operate / Validate
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    second_elem = cJSON_GetArrayItem(retrieved_arr, 1);
    second_value = cJSON_GetNumberValue(second_elem);
    printed = cJSON_PrintUnformatted(root);
    (void)second_value;
    (void)tmp;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}