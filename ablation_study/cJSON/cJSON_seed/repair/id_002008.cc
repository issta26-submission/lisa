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
//<ID> 2008
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
    cJSON *str_item = cJSON_CreateString("alpha");
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemReferenceToArray(arr, num_item);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddNullToObject(root, "optional");

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "note", cJSON_CreateString("metadata"));
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "single", cJSON_CreateNumber(7.0));

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "list"));
    cJSON *first = cJSON_GetArrayItem(cJSON_GetObjectItem(root, "list"), 0);
    cJSON *second = cJSON_GetArrayItem(cJSON_GetObjectItem(root, "list"), 1);
    char *first_str = cJSON_GetStringValue(first);
    double second_num = cJSON_GetNumberValue(second);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = first_str[0];
    buffer[1] = (char)('0' + (size % 10));
    buffer[2] = (char)('0' + ((int)second_num % 10));
    buffer[3] = buffer[0];
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}