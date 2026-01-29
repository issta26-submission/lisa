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
//<ID> 2004
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *child = cJSON_CreateObject();
    cJSON *item_num = cJSON_CreateNumber(2021.0);
    cJSON *item_str = cJSON_CreateString("hello");

    // step 2: Configure
    cJSON_AddItemToObject(child, "number", item_num);
    cJSON_AddItemToArray(array, cJSON_CreateString("first"));
    cJSON_AddItemToArray(array, item_str);
    cJSON_AddItemReferenceToArray(array, item_num);
    cJSON_AddItemToObject(root, "data", array);
    cJSON_AddItemToObject(root, "meta", child);
    cJSON_AddNullToObject(root, "optional");

    // step 3: Operate and Validate
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    int arr_size = cJSON_GetArraySize(array);
    cJSON *second = cJSON_GetArrayItem(array, 1);
    const char *sval = cJSON_GetStringValue(second);
    cJSON *refnum = cJSON_GetArrayItem(array, 2);
    double numval = cJSON_GetNumberValue(refnum);
    buffer[0] = sval && sval[0] ? sval[0] : 'x';
    buffer[1] = (char)('0' + ((int)numval % 10));
    buffer[2] = (char)('0' + (arr_size % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}