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
//<ID> 1486
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    char *refbuf = (char*)cJSON_malloc(16);
    refbuf[0] = 'i'; refbuf[1] = 'd'; refbuf[2] = 'x'; refbuf[3] = '\0';
    cJSON *sref = cJSON_CreateStringReference(refbuf);
    cJSON_AddItemToObject(root, "label", sref);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_bool arr_is_array = cJSON_IsArray(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);

    // step 3: Operate & Validate
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *printed_dup = cJSON_PrintUnformatted(dup);
    cJSON *dup_values = cJSON_GetObjectItem(dup, "values");
    cJSON_bool dup_is_array = cJSON_IsArray(dup_values);
    cJSON *dup_first = cJSON_GetArrayItem(dup_values, 0);
    double dup_first_val = cJSON_GetNumberValue(dup_first);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(printed_dup);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    cJSON_free(refbuf);

    // API sequence test completed successfully
    (void)arr_is_array;
    (void)first_val;
    (void)dup_is_array;
    (void)dup_first_val;
    return 66;
}