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
//<ID> 1899
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *container = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "container", container);
    cJSON *orig_array = cJSON_CreateArray();
    cJSON_AddItemToObject(container, "arr", orig_array);
    cJSON *item0 = cJSON_CreateString("zero");
    cJSON_InsertItemInArray(orig_array, 0, item0);
    cJSON *item_num = cJSON_CreateNumber(2.0);
    cJSON_InsertItemInArray(orig_array, 1, item_num);
    cJSON *item1 = cJSON_CreateString("one");
    cJSON_InsertItemInArray(orig_array, 1, item1);

    // step 2: Configure
    cJSON *new_array = cJSON_CreateArray();
    cJSON *na0 = cJSON_CreateString("alpha");
    cJSON_InsertItemInArray(new_array, 0, na0);
    cJSON *na1 = cJSON_CreateNumber(123.0);
    cJSON_InsertItemInArray(new_array, 1, na1);
    cJSON_ReplaceItemInObject(container, "arr", new_array);

    // step 3: Operate and Validate
    cJSON *got_arr = cJSON_GetObjectItem(container, "arr");
    cJSON *first = cJSON_GetArrayItem(got_arr, 0);
    cJSON *second = cJSON_GetArrayItem(got_arr, 1);
    char *s0 = cJSON_GetStringValue(first);
    double d1 = cJSON_GetNumberValue(second);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + ((s0 ? (unsigned char)s0[0] : (unsigned char)'X') % 10));
    buffer[2] = (char)('0' + (((int)d1) % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}