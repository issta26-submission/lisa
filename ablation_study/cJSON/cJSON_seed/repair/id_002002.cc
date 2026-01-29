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
//<ID> 2002
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(2021.0);
    cJSON *str_item = cJSON_CreateString("fuzz");
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(child, "year", num_item);
    cJSON_AddItemToObject(child, "label", str_item);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *arr_elem = cJSON_CreateString("elem");
    cJSON_AddItemToArray(arr, arr_elem);

    // step 2: Configure
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemReferenceToArray(arr, child_ref);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddNullToObject(root, "optional");

    // step 3: Operate and Validate
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    cJSON *got_year = cJSON_GetObjectItem(got_child, "year");
    double year = cJSON_GetNumberValue(got_year);
    cJSON *got_array = cJSON_GetObjectItem(root, "array");
    int arr_size = cJSON_GetArraySize(got_array);
    cJSON *first_elem = cJSON_GetArrayItem(got_array, 0);
    char *first_str = cJSON_GetStringValue(first_elem);
    cJSON *opt = cJSON_GetObjectItem(root, "optional");
    cJSON_bool was_null = cJSON_IsNull(opt);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = first_str ? first_str[0] : 'x';
    buffer[2] = (char)('0' + ((int)was_null % 10));
    buffer[3] = (char)('0' + ((int)((int)year % 10)));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}