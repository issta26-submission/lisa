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
//<ID> 2003
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
    cJSON *meta = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *ref_obj = cJSON_CreateObject();
    cJSON *ref_msg = cJSON_CreateString("referenced");
    cJSON_AddItemToObject(ref_obj, "msg", ref_msg);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemReferenceToArray(arr, ref_obj);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *null_item = cJSON_AddNullToObject(root, "optional");

    // step 2: Configure
    cJSON *name = cJSON_CreateString("nodeA");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *count = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(root, "count", count);

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON *got_msg = cJSON_GetObjectItem(second, "msg");
    char *got_str = cJSON_GetStringValue(got_msg);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = got_str[0];
    buffer[2] = (char)('0' + ((int)first_val % 10));
    buffer[3] = (char)('0' + (printed % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    (void)null_item;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}