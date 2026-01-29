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
//<ID> 1086
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
    cJSON *str_item = NULL;
    cJSON *false_item = NULL;
    cJSON *msg_item = NULL;
    cJSON *got_arr = NULL;
    cJSON *first_in_arr = NULL;
    char *printed = NULL;
    char *extracted_msg = NULL;
    char *extracted_first = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str_item);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "msg", cJSON_CreateString("world"));

    // step 3: Operate / Validate
    msg_item = cJSON_GetObjectItem(root, "msg");
    extracted_msg = cJSON_GetStringValue(msg_item);
    got_arr = cJSON_GetObjectItem(root, "arr");
    first_in_arr = cJSON_GetArrayItem(got_arr, 0);
    extracted_first = cJSON_GetStringValue(first_in_arr);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}