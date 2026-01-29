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
//<ID> 1138
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    double values[3] = {1.1, 2.2, 3.3};
    cJSON *dbl_array = cJSON_CreateDoubleArray(values, 3);
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON *name_ref = cJSON_CreateStringReference("example_name");
    cJSON_AddItemToObject(root, "values", dbl_array);
    cJSON_AddItemToObjectCS(root, "enabled", flag_true);
    cJSON_AddItemToObject(root, "name", name_ref);
    /* Use a duplicate of the array instead of an array reference to avoid
       sharing/free issues when both are added to the same parent. */
    cJSON *values_ref = cJSON_Duplicate(dbl_array, 1);
    cJSON_AddItemToObject(root, "values_ref", values_ref);
    cJSON_bool is_true = cJSON_IsTrue(flag_true);
    cJSON *first_item = cJSON_GetArrayItem(dbl_array, 0);
    double first_num = cJSON_GetNumberValue(first_item);
    cJSON_AddNumberToObject(root, "first_plus_flag", first_num + (double)is_true);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
}