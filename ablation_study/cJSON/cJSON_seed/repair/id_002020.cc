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
//<ID> 2020
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
    cJSON_AddItemToObject(root, "values", array);
    cJSON_AddTrueToObject(root, "active");
    cJSON_AddNumberToObject(root, "threshold", 3.14);
    cJSON *elem_str = cJSON_CreateString("first");
    cJSON_AddItemToArray(array, elem_str);
    cJSON *elem_num = cJSON_CreateNumber(42.0);
    cJSON_InsertItemInArray(array, 0, elem_num);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "version", 2.0);

    // step 3: Operate and Validate
    int sz = cJSON_GetArraySize(array);
    cJSON *it0 = cJSON_GetArrayItem(array, 0);
    cJSON *it1 = cJSON_GetArrayItem(array, 1);
    double numv = cJSON_GetNumberValue(it0);
    char *strv = cJSON_GetStringValue(it1);
    char *printed = cJSON_Print(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = strv[0];
    buffer[2] = (char)('0' + ((int)numv % 10));
    buffer[3] = (char)('0' + (sz % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}