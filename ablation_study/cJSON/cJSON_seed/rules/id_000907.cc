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
//<ID> 907
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON *retrieved0 = NULL;
    cJSON *parsed = NULL;
    cJSON *parsed_arr = NULL;
    char *printed = NULL;
    char *copied = NULL;
    cJSON_bool is_true0 = 0;
    cJSON_bool is_true1 = 0;
    cJSON_bool parsed_third_true = 0;
    int arr_size = 0;
    char buf[16];
    memset(buf, 0, sizeof(buf));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "flags", arr);
    item0 = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, item0);
    item1 = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, item1);
    item2 = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, item2);
    cJSON_AddNumberToObject(root, "count", 3.0);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    parsed = cJSON_Parse("{\"arr2\":[true,false,true],\"num\":123}");
    parsed_arr = cJSON_GetObjectItem(parsed, "arr2");
    retrieved0 = cJSON_GetArrayItem(arr, 0);
    is_true0 = cJSON_IsTrue(retrieved0);
    is_true1 = cJSON_IsTrue(cJSON_GetArrayItem(arr, 1));
    parsed_third_true = cJSON_IsTrue(cJSON_GetArrayItem(parsed_arr, 2));
    arr_size = cJSON_GetArraySize(arr);
    copied = (char*)cJSON_malloc(16);
    memset(copied, 0, 16);
    copied[0] = is_true0 ? '1' : '0';
    copied[1] = is_true1 ? '1' : '0';
    copied[2] = parsed_third_true ? '1' : '0';
    copied[3] = (char)('0' + (arr_size % 10));
    (void)buf;
    (void)retrieved0;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(copied);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}