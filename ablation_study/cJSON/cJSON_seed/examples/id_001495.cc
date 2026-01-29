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
//<ID> 1495
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "values", arr);

    /* Duplicate the array and create a reference to the duplicate to avoid
       double-free when both the original and a reference are attached to root. */
    cJSON *arr_copy = cJSON_Duplicate(arr, cJSON_True);
    cJSON *arr_ref = cJSON_CreateArrayReference(arr_copy);
    cJSON_AddItemToObject(root, "values_ref", arr_ref);

    char *printed = cJSON_PrintUnformatted(root);
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    char *first_str = cJSON_GetStringValue(first_item);

    cJSON *ref_obj = cJSON_GetObjectItem(root, "values_ref");
    cJSON *ref_first = cJSON_GetArrayItem(ref_obj, 0);
    char *ref_first_str = cJSON_GetStringValue(ref_first);
    (void)first_str;
    (void)ref_first_str;

    cJSON_free(printed);
    cJSON_Delete(root);

    return 66;
}