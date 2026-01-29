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
//<ID> 206
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
    cJSON_AddItemToObject(root, "items", array);
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON *num2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, str1);
    cJSON_AddItemToArray(array, num2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "flag", flag);

    // step 2: Configure
    char *buffer1 = cJSON_PrintBuffered(root, 64, 1);
    void *tmp_buf = cJSON_malloc(128);
    memset(tmp_buf, 0, 128);
    cJSON *parsed = cJSON_Parse(buffer1);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemViaPointer(array, str1);
    cJSON_AddItemToObject(root, "moved", detached);
    char *buffer2 = cJSON_PrintBuffered(root, 32, 0);
    int remaining = cJSON_GetArraySize(array);
    cJSON *first = cJSON_GetArrayItem(array, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *moved_item = cJSON_GetObjectItem(root, "moved");
    char *moved_str = cJSON_GetStringValue(moved_item);
    (void)moved_str;
    cJSON *duplicate = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, duplicate, 0);
    (void)remaining;
    (void)first_val;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(buffer1);
    cJSON_free(buffer2);
    cJSON_free(tmp_buf);
    cJSON_Delete(parsed);
    cJSON_Delete(duplicate);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}