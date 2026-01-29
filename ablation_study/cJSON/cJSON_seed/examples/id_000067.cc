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
//<ID> 67
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *existing_num = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObjectCS(root, "existing", existing_num);

    // step 2: Configure
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.72);
    cJSON *n3 = cJSON_CreateNumber(-1.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON *imported = cJSON_Parse("{\"imported\":42}");
    cJSON_AddItemToObjectCS(root, "import", imported);

    // step 3: Operate & Validate
    cJSON_bool has_values = cJSON_HasObjectItem(root, "values");
    cJSON_bool has_existing = cJSON_HasObjectItem(root, "existing");
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    double v0 = cJSON_GetNumberValue(first);
    double v1 = cJSON_GetNumberValue(second);
    double sum_val = v0 + v1 + (double)has_existing;
    cJSON *sum_item = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObjectCS(root, "sum", sum_item);
    cJSON *status_num = cJSON_CreateNumber((double)has_values);
    cJSON_AddItemToObject(root, "status", status_num);

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}