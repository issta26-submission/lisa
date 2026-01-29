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
//<ID> 913
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *nested = NULL;
    cJSON *arr = NULL;
    cJSON *sref0 = NULL;
    cJSON *sref1 = NULL;
    cJSON *got_nested = NULL;
    cJSON *got_arr = NULL;
    cJSON *first_item = NULL;
    const char *first_str = NULL;
    int arr_size = 0;
    char *buffer = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "nested", nested);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(nested, "arr", arr);
    sref0 = cJSON_CreateStringReference("one");
    cJSON_AddItemToArray(arr, sref0);
    sref1 = cJSON_CreateStringReference("two");
    cJSON_AddItemToArray(arr, sref1);

    // step 3: Operate / Validate
    got_nested = cJSON_GetObjectItemCaseSensitive(root, "nested");
    got_arr = cJSON_GetObjectItemCaseSensitive(got_nested, "arr");
    arr_size = cJSON_GetArraySize(got_arr);
    first_item = cJSON_GetArrayItem(got_arr, 0);
    first_str = cJSON_GetStringValue(first_item);
    buffer = (char *)cJSON_malloc(16);
    memset(buffer, 0, 16);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = first_str ? first_str[0] : '\0';

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(nested, "arr");
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)got_nested;
    (void)got_arr;
    (void)first_item;
    (void)first_str;
    (void)sref0;
    (void)sref1;
    return 66;
}